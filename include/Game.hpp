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
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;
    static const float UPDATES_PER_SEC;
    static const float UPDATE_INTERVAL;
    static const float MAX_ASTEROIDS_PER_SEC;
    static const float MAX_UPDATE_TIME_INCREASE;
    static const float TIME_TO_HIGHEST_LEVEL;

    std::unique_ptr<Window> m_window;
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
    void resetGame();

    void gameLoop();
    void processInput();
    void update(float deltaTime);
    void render();
    void checkForCollisions();
    void createAsteroid();
    bool objectLeftWindow(std::shared_ptr<GameObject> gameObject);
};

#endif