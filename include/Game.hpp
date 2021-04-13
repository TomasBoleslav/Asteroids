#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Asteroid.hpp"

#include <memory>
#include <vector>

class Game final
{
public:
    Game();
    ~Game();

    void run();

private:
    enum class GameState { Start, Running, Over };

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    const glm::vec2 SCR_SIZE = glm::vec2(SCR_WIDTH, SCR_HEIGHT);
    const glm::vec2 SCR_CENTER = SCR_SIZE / 2.0f;

    const float UPDATES_PER_SEC = 60.0;
    const float UPDATE_INTERVAL = 1.0f / UPDATES_PER_SEC;
    const double TIME_BETWEEN_STATES = 1.0;

    const std::size_t ASTEROID_MIN_COUNT = 5;
    const float ASTEROID_MIN_ROT_SPEED = -30.0f;
    const float ASTEROID_MAX_ROT_SPEED = 30.0f;
    const float ASTEROID_MIN_SPEED = 70.0f;
    const float ASTEROID_MAX_SPEED = 200.0f;
    const float ASTEROID_MIN_ANGLE = 20.0f;
    const float ASTEROID_MAX_ANGLE = 40.0f;
    const float ASTEROID_SIZE = 40.0f;

    const glm::vec2 PLAYER_SIZE = glm::vec2(28.0f, 35.0f);
    const float PLAYER_FORCE = 400.0f;
    const float PLAYER_DECAY = 0.99f;
    const float PLAYER_TURN_SPEED = 250.0f;
    const float PLAYER_RELOAD_TIME = 0.3f;

    const float BULLET_SPEED = 400.0f;
    const glm::vec2 BULLET_SIZE = glm::vec2(3.0f, 10.0f);
    const float BULLET_RANGE = std::min(SCR_SIZE.x, SCR_SIZE.y);
    const double BULLET_LIFETIME = BULLET_RANGE / BULLET_SPEED;

    const glm::vec2 LEVEL_ICON_SIZE = glm::vec2(16.0f, 20.0f);
    const glm::vec3 LEVEL_ICON_COLOR = glm::vec3(0.5f, 0.5f, 0.5f);
    const float LEVEL_ICON_OFFSET = 20.0f;
    const std::size_t LEVEL_ICONS_IN_ROW = 5;

    std::unique_ptr<Window> m_window;

    std::size_t m_level;
    GameState m_state;
    Timer m_stateTimer;

    Renderer m_renderer;

    std::shared_ptr<Player> m_player;
    std::vector<std::shared_ptr<Asteroid>> m_asteroids;
    std::vector<std::shared_ptr<Bullet>> m_bullets;

    void init();
    void createWindow();
    void loadResources();
    void setCommonUniforms();
    void createPlayer();
    void restartGame();

    void gameLoop();
    void processInput();
    void update(float deltaTime);
    void handleCollisions();
    void render();
    void renderLevelCount();
    
    void determineState();
    void gameOver();
    void increaseLevel();

    void spawnAsteroids();
    void createAsteroid();
    glm::vec2 getAsteroidRandomPos(float size);
    void shootBullet();

    void handleStrayObjects();
    void moveObjectBack(const std::shared_ptr<GameObject>& gameObject);
    void removeBulletsOutsideRange();

    template<typename T, typename F>
    void removeObjectsIf(std::vector<std::shared_ptr<T>>& objects, F function);
};

template<typename T, typename F>
void Game::removeObjectsIf(std::vector<std::shared_ptr<T>>& objects, F function)
{
    objects.erase(std::remove_if(objects.begin(), objects.end(), function), objects.end());
}

#endif