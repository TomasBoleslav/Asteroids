#include "Game.hpp"

#include "Debug.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Input.hpp"
#include "Random.hpp"
#include "Geometry.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <stdexcept>
#include <algorithm>

Game::Game() : m_level(1), m_state(GameState::Running)
{
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
    rnd::setSeed(1);
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
    m_player->size = PLAYER_SIZE;
    m_player->reloadTime = PLAYER_RELOAD_TIME;
    m_player->force = PLAYER_FORCE;
    m_player->decay = PLAYER_DECAY;
    m_player->turnSpeed = PLAYER_TURN_SPEED;
    m_player->bounds = {
        glm::vec2(0.0f, 1.0f), glm::vec2(0.5f, 0.75f),
        glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.0f)
    };
}

void Game::restartGame()
{
    m_level = 1;
    m_player->position = SCR_CENTER;
    m_player->velocity = glm::vec2(0.0f);
    m_player->rotation = 0.0f;
    m_asteroids.clear();
    m_bullets.clear();
    spawnAsteroids();
    m_state = GameState::Start;
    m_stateTimer.start(TIME_BETWEEN_STATES);
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
        glfwPollEvents();
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
    renderLevelCount();
    m_window->swapBuffers();
}

void Game::renderLevelCount()
{
    auto texture = ResourceManager::getTexture("ship");
    glm::vec2 pos = glm::vec2(0.0f);
    for (size_t i = 0; i < m_level; i++)
    {
        if (i % LEVEL_ICONS_IN_ROW == 0)
        {
            pos.x = LEVEL_ICON_OFFSET;
            pos.y += LEVEL_ICON_SIZE.y;
        }
        else
        {
            pos.x += LEVEL_ICON_SIZE.x;
        }
        m_renderer.drawQuad(texture, pos, LEVEL_ICON_SIZE, 0.0f, LEVEL_ICON_COLOR);
    }
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
    if (m_state != GameState::Running)
    {
        return;
    }
    handleCollisions();
    if (m_asteroids.size() == 0)
    {
        increaseLevel();
    }
    m_player->update(deltaTime);
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
    std::size_t count = m_level * ASTEROID_MIN_COUNT;
    for (size_t i = 0; i < count; i++)
    {
        createAsteroid();
    }
}

void Game::handleCollisions()
{
    removeObjectsIf(m_asteroids,
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
    );
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
    auto asteroid = std::make_shared<Asteroid>();
    asteroid->texture = ResourceManager::getTexture("asteroid");
    asteroid->size = glm::vec2(ASTEROID_SIZE);
    asteroid->position = getAsteroidRandomPos(ASTEROID_SIZE);
    asteroid->rotation = rnd::getFloat(0.0f, 360.0f);
    asteroid->rotationSpeed = rnd::getFloat(ASTEROID_MIN_ROT_SPEED, ASTEROID_MAX_ROT_SPEED);
    float speed = rnd::getFloat(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED);
    float velocityAngle = rnd::getIndex(3) * 90.0f + rnd::getFloat(ASTEROID_MIN_ANGLE, ASTEROID_MAX_ANGLE);
    asteroid->velocity = speed * geom::getDirection(velocityAngle);
    asteroid->bounds = {
        glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.5f), glm::vec2(0.75f, 1.0f),
        glm::vec2(0.25f, 1.0f), glm::vec2(0.0f, 0.75f), glm::vec2(0.15f, 0.25f)
    };
    m_asteroids.push_back(asteroid);
}

glm::vec2 Game::getAsteroidRandomPos(float size)
{
    float randomX = rnd::getFloat(-size, SCR_WIDTH + size);
    float randomY = rnd::getFloat(-size, SCR_HEIGHT + size);
    return rnd::choose<glm::vec2>(
        glm::vec2(randomX, -size),  // top
        glm::vec2(-size, randomY)   // left
        );
}

void Game::gameOver()
{
    m_state = GameState::Over;
    m_stateTimer.start(TIME_BETWEEN_STATES);
}

void Game::shootBullet()
{
    auto bullet = m_player->shoot(BULLET_SIZE, BULLET_SPEED, BULLET_LIFETIME);
    m_bullets.push_back(bullet);
}

void Game::handleStrayObjects()
{
    removeObjectsIf(m_bullets, [](const std::shared_ptr<Bullet>& bullet) { return bullet->isDestroyed(); });
    for (auto&& bullet : m_bullets)
    {
        moveObjectBack(bullet);
    }
    for (auto&& asteroid : m_asteroids)
    {
        moveObjectBack(asteroid);
    }
    moveObjectBack(m_player);
}

void Game::moveObjectBack(const std::shared_ptr<GameObject>& gameObject)
{
    glm::vec2 pos = gameObject->position;
    glm::vec2 size = gameObject->size;
    if (pos.x < -size.x)
    {
        pos.x = SCR_SIZE.x;
    }
    else if (pos.x > SCR_SIZE.x)
    {
        pos.x = -size.x;
    }
    if (pos.y < -size.y)
    {
        pos.y = SCR_SIZE.y;
    }
    else if (pos.y > SCR_HEIGHT)
    {
        pos.y = -size.y;
    }
    gameObject->position = pos;
}

