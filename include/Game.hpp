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
    const glm::vec2 SCR_CENTER = glm::vec2(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

    const float UPDATES_PER_SEC = 60.0;
    const float UPDATE_INTERVAL = 1.0f / UPDATES_PER_SEC;

    const std::size_t START_ASTEROID_COUNT = 8;
    const std::size_t ASTEROID_COUNT_INCREASE = 4;

    const std::size_t MIN_ASTEROIDS = 5;
    const std::size_t MAX_ASTEROIDS = 30;

    const float ASTEROID_MIN_ROT_SPEED = 10.0f;
    const float ASTEROID_MAX_ROT_SPEED = 30.0f;
    const float MIN_ASTEROID_SPEED = 50.0f;
    const float MAX_ASTEROID_SPEED = 150.0f;
    const glm::vec2 MIN_ASTEROID_DIRECTION = glm::vec2();
    const float MIN_ASTEROID_ANGLE = 20.0f;
    const float MAX_ASTEROID_ANGLE = 40.0f;


    const float BULLET_SPEED = 30.0f;

    const float SHIP_TURN_SPEED = 30.0f;
    const float SHIP_FORCE = 100.0f;

    const double TIME_TO_NEXT_LEVEL = 60.0f;
    const double TIME_BETWEEN_STATES = 3.0;

    std::unique_ptr<Window> m_window;

    std::size_t m_level;

    GameState m_state;
    Timer m_stateTimer;
    Timer m_levelTimer;

    Renderer m_renderer;

    std::shared_ptr<Player> m_player;
    std::vector<std::shared_ptr<Asteroid>> m_asteroids;
    std::vector<std::shared_ptr<Bullet>> m_bullets;

    std::vector<float> m_asteroidSizes;
    float timeToNextAsteroid;
    float asteroidsPerSec;
    float maxAsteroidsPerSec;
    float updateTimeIncrease;
    float maxUpdateTimeIncrease;

    void init();
    void createWindow();
    void loadResources();
    void setCommonUniforms();
    void createPlayer();
    void restartGame();

    void gameLoop();
    void processInput();
    void determineState();
    void update(float deltaTime);
    void render();

    void handleCollisions();
    void spawnAsteroids();
    void createAsteroid();

    glm::vec2 getAsteroidRandomPos(float size);
    void gameOver();

    void shootBullet();

    void handleStrayObjects();
    void moveObjectBack(const std::shared_ptr<GameObject>& gameObject);
    glm::vec2 getObjectMirrorPosition(glm::vec2 position, glm::vec2 size);

    void increaseLevel();

    template<typename T>
    void removeStrayObjects(std::vector<std::shared_ptr<T>>& objects);

    bool objectLeftWindow(std::shared_ptr<GameObject> gameObject);

    template<typename T, typename F>
    void forObjects(const std::vector<std::shared_ptr<T>>& objects, F function);

};

#endif