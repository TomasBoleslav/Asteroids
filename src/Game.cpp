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
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <algorithm>
#include <cstdlib>

Game::Game() : timeToNextAsteroid(1.0f), asteroidsPerSec(5.0f), m_level(0)
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
    createPlayer();
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
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("simple")->use();
    ResourceManager::getShader("simple")->setMat4("u_projection", projection);
}

void Game::createPlayer()
{
    m_player = std::make_shared<Player>();
    m_player->texture = ResourceManager::getTexture("ship");
    m_player->size = glm::vec2(40.0f, 50.0f);
    m_player->bounds = {
        glm::vec2(0.0f, 1.0f), glm::vec2(0.5f, 0.75f),
        glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.0f)
    };
    m_player->reloadTime = 1.0f;
    m_player->forceValue = 400.0f;
    m_player->decay = 0.99f;
    m_player->turnSpeed = 180.0f;
}

void Game::restartGame()
{
    m_state = GameState::Start;
    m_stateTimer.start(TIME_BETWEEN_STATES);
    m_level = 0;
    m_player->position = SCR_CENTER;
    m_player->velocity = glm::vec2(0.0f);
    m_player->rotation = 0.0f;
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
    if (Input::isKeyPressed(GLFW_KEY_SPACE) && m_state == GameState::Running && m_player->canShoot())
    {
        shootBullet();
    }
    m_player->processInput(); 
}

void Game::render()
{
    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    auto background = ResourceManager::getTexture("background");
    m_renderer.drawQuad(background, glm::vec2(0.0f), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

    if (m_state != GameState::Over)
    {
        m_player->draw(m_renderer);
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
        handleCollisions();
        if (m_asteroids.size() == 0)
        {
            //increaseLevel();
        }
    }
    if (m_state != GameState::Over)
    {
        m_player->update(deltaTime);
    }
    for (auto&& asteroid : m_asteroids)
    {
        asteroid->update(deltaTime);
    }
    for (auto&& bullet : m_bullets)
    {
        bullet->update(deltaTime);
    }
    handleStrayObjects();
}

void Game::increaseLevel()
{
    ++m_level;
    spawnAsteroids();
}

void Game::spawnAsteroids()
{
    std::size_t count = ASTEROID_MIN_COUNT + m_level * ASTEROID_COUNT_INCREASE;
    count = std::min(count, ASTEROID_MAX_COUNT); // TODO: nechat neomezene asteroidu?
    for (size_t i = 0; i < count; i++)
    {
        createAsteroid();
    }
}

void Game::handleCollisions()
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
        if (m_player->collidesWith(asteroid))
        {
            gameOver();
        }
    }
}

void Game::createAsteroid()
{
    std::shared_ptr<Asteroid> asteroid = std::make_shared<Asteroid>();
    float size = random::chooseFromVector<float>(m_asteroidSizes);
    float speed = random::getFloat(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED);
    float velocityAngle = random::getSizeT(3) * 90.0f + random::getFloat(ASTEROID_MIN_ANGLE, ASTEROID_MAX_ANGLE);
    asteroid->texture = ResourceManager::getTexture("asteroid");
    asteroid->size = glm::vec2(size);
    asteroid->position = getAsteroidRandomPos(size);
    asteroid->rotation = random::getFloat(0.0f, 360.0f);
    asteroid->velocity = speed * geom::getDirection(velocityAngle);
    asteroid->angularVelocity = random::getFloat(ASTEROID_MIN_ROT_SPEED, ASTEROID_MAX_ROT_SPEED);
    asteroid->bounds = {
        glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.5f), glm::vec2(0.75f, 1.0f),
        glm::vec2(0.25f, 1.0f), glm::vec2(0.0f, 0.75f), glm::vec2(0.15f, 0.25f)
    };
    m_asteroids.push_back(asteroid);
}

glm::vec2 Game::getAsteroidRandomPos(float size)
{
    float randomX = random::getFloat(-size, SCR_WIDTH + size);
    float randomY = random::getFloat(-size, SCR_HEIGHT + size);
    return random::choose<glm::vec2>(
        glm::vec2(randomX, -size),  // top
        glm::vec2(-size, randomY)   // left
    );
}

template<typename T>
void Game::removeStrayObjects(std::vector<std::shared_ptr<T>>& objects)
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
    auto bullet = m_player->shoot(glm::vec2(4.0, 20.0), BULLET_SPEED);
    m_bullets.push_back(bullet);
}

void Game::handleStrayObjects()
{
    for (auto&& asteroid : m_asteroids)
    {
        moveObjectBack(asteroid);
    }
    moveObjectBack(m_player);
    removeStrayObjects(m_bullets);
}

void Game::moveObjectBack(const std::shared_ptr<GameObject>& gameObject)
{
    glm::vec2 pos = gameObject->position;
    glm::vec2 size = gameObject->size;
    if (pos.x < -size.x)
    {
        pos.x = SCR_WIDTH;
    }
    else if (pos.x > SCR_WIDTH)
    {
        pos.x = -size.x;
    }
    if (pos.y < -size.y)
    {
        pos.y = SCR_HEIGHT;
    }
    else if (pos.y > SCR_HEIGHT)
    {
        pos.y = -size.y;
    }
    gameObject->position = pos;
}

