#include "Game.hpp"

#include "Errors.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Input.hpp"
#include "Random.hpp"
#include "Geometry.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <stdexcept>
#include <algorithm>
#include <cstdlib>

Game::Game() : timeToNextAsteroid(1.0f), asteroidsPerSec(5.0f)
{
    m_asteroidSizes = { 30.0, 50.0, 70.0 };
}

Game::~Game()
{
    ResourceManager::clear();
}

void Game::run()
{
    init();
    restartGame();
    gameLoop();
}

void Game::init()
{
    createWindow();
    loadResources();
    setCommonUniforms();
    m_renderer.init(ResourceManager::getShader("simple"));
    initPlayer();
    random::setSeed(1);
}

void Game::createWindow()
{
    Window::setHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    Window::setHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    Window::setHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    Window::setHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef __APPLE__
    Window::setHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_window = std::make_unique<Window>(SCR_WIDTH, SCR_HEIGHT, "SpaceGame");
    GL_CALL(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Game::loadResources()
{
    ResourceManager::loadShader("simple", "res/shaders/simple.vert", "res/shaders/simple.frag");
    ResourceManager::loadTexture("ship", "res/images/ship.png", true);
    ResourceManager::loadTexture("asteroid", "res/images/asteroid.png", true);
    ResourceManager::loadTexture("background", "res/images/background.png", true);
}

void Game::setCommonUniforms()
{
    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("simple")->use();
    ResourceManager::getShader("simple")->setMat4("u_projection", projection);
}

void Game::initPlayer()
{
    m_player.texture = ResourceManager::getTexture("ship");
    m_player.size = glm::vec2(40.0f, 50.0f);
    m_player.bounds = {
        glm::vec2(0.0f, 1.0f), glm::vec2(0.5f, 0.75f),
        glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.0f)
    };
    m_player.shootDelay = 1.0f;
    // TODO: set force and friction
}

void Game::restartGame()
{
    m_state = GameState::Start;
    m_stateTimer.start(TIME_BETWEEN_STATES);
    m_player.position = SCR_CENTER;
    m_player.velocity = glm::vec2(0.0f);
    m_asteroids.clear();
    m_bullets.clear();
}

void Game::gameLoop()
{
    double lastUpdated = glfwGetTime();
    while (!m_window->shouldClose())
    {
        determineState();
        processInput();
        double currentTime = glfwGetTime();
        while (lastUpdated + UPDATE_INTERVAL <= currentTime)
        {
            lastUpdated += UPDATE_INTERVAL;
            update(UPDATE_INTERVAL);
        }
        render();
        glfwPollEvents();   // TODO: add to window
    }
}

void Game::processInput()
{
    if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
    {
        m_window->setToClose();
        return;
    }
    if (Input::isKeyPressed(GLFW_KEY_SPACE) && m_state == GameState::Running && m_player.canShoot())
    {
        shootBullet();
    }
    m_player.processInput(); 
}

void Game::render()
{
    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    auto background = ResourceManager::getTexture("background");
    m_renderer.drawQuad(background, glm::vec2(0.0f), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

    if (m_state != GameState::Over)
    {
        m_player.draw(m_renderer);
    }
    for (auto&& asteroid : m_asteroids)
    {
        asteroid->draw(m_renderer);
    }
    for (auto&& bullet : m_bullets)
    {
        bullet->draw(m_renderer);
    }
    m_window->swapBuffers();
}

void Game::determineState()
{
    switch (m_state)
    {
    case GameState::Start:
        if (m_stateTimer.finished())
        {
            m_state = GameState::Running;
        }
        break;
    case GameState::Over:
        if (m_stateTimer.finished())
        {
            restartGame();
        }
        break;
    }
}

void Game::update(float deltaTime)
{
    if (m_state == GameState::Running)
    {
        checkForCollisions();
        spawnAsteroids(deltaTime);
    }
    if (m_state != GameState::Over)
    {
        m_player.update(deltaTime);
    }
    for (auto&& asteroid : m_asteroids)
    {
        asteroid->update(deltaTime);
    }
    for (auto&& bullet : m_bullets)
    {
        bullet->update(deltaTime);
    }
    removeLeftObjects(m_asteroids);
    removeLeftObjects(m_bullets);
}

void Game::spawnAsteroids(float deltaTime)
{
    timeToNextAsteroid -= deltaTime;
    while (timeToNextAsteroid < 0.0f)
    {
        createAsteroid();
        timeToNextAsteroid += 1.0f / asteroidsPerSec; // TODO: correct
    }
}

void Game::checkForCollisions()
{
    m_asteroids.erase(std::remove_if(m_asteroids.begin(), m_asteroids.end(),
        [this](const std::shared_ptr<Asteroid>& asteroid)
        {
            for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
            {
                if ((*it)->collidesWith(asteroid))
                {
                    m_bullets.erase(it);
                    return true;
                }
            }
            return false;
        }
        ), m_asteroids.end());
    for (auto&& asteroid : m_asteroids)
    {
        if (m_player.collidesWith(asteroid))
        {
            gameOver();
        }
    }
}

void Game::createAsteroid()
{
    const float minSpeed = 5000.0f;
    const float maxSpeed = 6000.0f;
    const float minAngVelocity = -30.0f;
    const float maxAngVelocity = 30.0f;
    float size = m_asteroidSizes[random::getSizeT(m_asteroidSizes.size() - 1)];
    glm::vec2 position = glm::vec2(random::getFloat(0.0f, SCR_WIDTH), -2.0f * size);
    glm::vec2 dirLeft = glm::vec2(0.0f, SCR_HEIGHT) - position;
    glm::vec2 dirRight = glm::vec2(SCR_WIDTH, SCR_HEIGHT) - position;
    glm::vec2 direction = glm::normalize(random::getVec2(dirLeft, dirRight));

    std::shared_ptr<Asteroid> asteroid = std::make_shared<Asteroid>();
    asteroid->texture = ResourceManager::getTexture("asteroid");
    asteroid->size = glm::vec2(size);
    asteroid->position = position;
    asteroid->rotation = random::getFloat(0.0f, 360.0f);
    asteroid->velocity = random::getFloat(minSpeed, maxSpeed) * direction;
    asteroid->angularVelocity = random::getFloat(minAngVelocity, maxAngVelocity);
    asteroid->bounds = {
        glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.5f), glm::vec2(0.75f, 1.0f),
        glm::vec2(0.25f, 1.0f), glm::vec2(0.0f, 0.75f), glm::vec2(0.15f, 0.25f)
    };
    m_asteroids.push_back(asteroid);
}

template<typename T>
void Game::removeLeftObjects(std::vector<std::shared_ptr<T>>& objects)
{
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [this](const std::shared_ptr<T>& gameObject)
        {
            return objectLeftWindow(gameObject);
        }
    ), objects.end());
}


bool Game::objectLeftWindow(std::shared_ptr<GameObject> gameObject)
{
    glm::vec2 pos = gameObject->position;
    glm::vec2 windowMiddle = glm::vec2(SCR_WIDTH / 2, SCR_HEIGHT / 2);
    return glm::length(pos - windowMiddle) > 2 * std::max(SCR_WIDTH, SCR_HEIGHT);
}

void Game::gameOver()
{
    m_state = GameState::Over;
    m_stateTimer.start(TIME_BETWEEN_STATES);
}

template<typename T, typename F>
void Game::forObjects(const std::vector<std::shared_ptr<T>>& objects, F function)
{
    std::for_each(objects.begin(), objects.end(), function);
}

void Game::shootBullet()
{
    auto bullet = m_player.shoot();
    bullet->speed = 100.0f;
    bullet->size = glm::vec2(50.0f, 50.0f);
    bullet->bounds = {
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
    };
    bullet->position = m_player.position + glm::vec2((m_player.size.x - bullet->size.x) / 2, 0.0f);
    m_bullets.push_back(bullet);
}