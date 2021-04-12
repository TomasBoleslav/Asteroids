#include "Game.hpp"

#include "Errors.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <stdexcept>
#include <algorithm>
#include <cstdlib>

const float Game::UPDATES_PER_SEC = 60.0;
const float Game::UPDATE_INTERVAL = 1 / UPDATES_PER_SEC;

Game::Game()
{
}

Game::~Game()
{
    ResourceManager::clear();
}

void Game::run()
{
    init();
    resetGame();
    gameLoop();
}

void Game::init()
{
    createWindow();
    loadResources();
    setCommonUniforms();
    m_renderer.init(ResourceManager::getShader("simple"));
    initPlayer();


    for (size_t i = 0; i < 10; i++)
    {
        std::shared_ptr<Asteroid> asteroid = std::make_shared<Asteroid>();
        asteroid->texture = ResourceManager::getTexture("asteroid");
        asteroid->size = glm::vec2(64.0f, 64.0f);
        asteroid->position = (float)i * glm::vec2(50.0f, 80.0f);
        asteroid->velocity = 0.0f * glm::vec2(0.0, 1.0);
        asteroid->angularVelocity = 30.0f;
        asteroid->bounds = {
            glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.5f), glm::vec2(0.75f, 1.0f),
            glm::vec2(0.25f, 1.0f), glm::vec2(0.0f, 0.75f), glm::vec2(0.15f, 0.25f)
        };
        m_asteroids.push_back(asteroid);
    }
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    m_player.shootDelay = 0.5f;
    // TODO: set force and friction
}

void Game::resetGame()
{
    m_player.position.x = (SCR_WIDTH - m_player.size.x) / 2;
    m_player.position.y = 3 * SCR_HEIGHT / 4 - m_player.size.y;
}

void Game::gameLoop()
{
    double lastUpdated = glfwGetTime();
    while (!m_window->shouldClose())
    {
        processInput();
        double currentTime = glfwGetTime();
        while (lastUpdated + UPDATE_INTERVAL <= currentTime)
        {
            checkForCollisions();
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
    if (Input::isKeyPressed(GLFW_KEY_SPACE) && m_player.canShoot())
    {
        auto bullet = m_player.shoot();
        bullet->speed += 400.0f;
        bullet->size = glm::vec2(50.0f, 50.0f);
        bullet->bounds = {
            glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
        };
        bullet->position = m_player.position + glm::vec2((m_player.size.x - bullet->size.x) / 2, 0.0f);
        m_bullets.push_back(bullet);
    }
    m_player.processInput();
    
}

void Game::render()
{
    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    auto background = ResourceManager::getTexture("background");
    m_renderer.drawQuad(background, glm::vec2(0.0f), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

    m_player.draw(m_renderer);
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


void Game::update(float deltaTime)
{
    m_player.update(deltaTime);
    for (auto&& asteroid : m_asteroids)
    {
        asteroid->update(deltaTime);
    }
    for (auto&& bullet : m_bullets)
    {
        bullet->update(deltaTime);
    }
    //checkForCollisions();
}

void Game::checkForCollisions()
{
    std::size_t i = 0;
    while (i < m_asteroids.size())
    {
        if (m_player.collidesWith(m_asteroids[i]))
        {
            m_asteroids.erase(m_asteroids.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void Game::createAsteroid()
{/*
    const int minSize = 30;
    const int maxSize = 100;
    float size = rand(maxSize);
    std::shared_ptr<Asteroid> asteroid = std::make_shared<Asteroid>();
    asteroid->texture = ResourceManager::getTexture("asteroid");
    asteroid->size = glm::vec2(64.0f, 64.0f);
    asteroid->position = glm::vec2(50.0f, 80.0f);
    asteroid->velocity = 0.0f * glm::vec2(0.0, 1.0);
    asteroid->angularVelocity = 30.0f;
    asteroid->bounds = {
        glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.5f), glm::vec2(0.75f, 1.0f),
        glm::vec2(0.25f, 1.0f), glm::vec2(0.0f, 0.75f), glm::vec2(0.15f, 0.25f)
    };
    m_asteroids.push_back(asteroid);*/
}

bool Game::objectLeftWindow(std::shared_ptr<GameObject> gameObject)
{
    glm::vec2 pos = gameObject->position;
    glm::vec2 windowMiddle = glm::vec2(SCR_WIDTH / 2, SCR_HEIGHT / 2);
    return glm::length(pos - windowMiddle) > 2 * std::max(SCR_WIDTH, SCR_HEIGHT);
}