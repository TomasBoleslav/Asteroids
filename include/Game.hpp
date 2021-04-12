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
    const float MAX_ASTEROIDS_PER_SEC = 0.0f;
    const float MAX_UPDATE_TIME_INCREASE = 0.0f;
    const float TIME_TO_HIGHEST_LEVEL = 0.0f;
    const double TIME_BETWEEN_STATES = 3.0;

    std::unique_ptr<Window> m_window;

    GameState m_state;
    Timer m_stateTimer;

    Renderer m_renderer;

    Player m_player;
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
    void initPlayer();
    void restartGame();

    void gameLoop();
    void processInput();
    void update(float deltaTime);
    void render();
    void checkForCollisions();
    void createAsteroid();

    void spawnAsteroids(float deltaTime);

    void determineState();
    void gameOver();

    void shootBullet();

    template<typename T>
    void removeLeftObjects(std::vector<std::shared_ptr<T>>& objects);

    bool objectLeftWindow(std::shared_ptr<GameObject> gameObject);

    template<typename T, typename F>
    void forObjects(const std::vector<std::shared_ptr<T>>& objects, F function);

};

#endif