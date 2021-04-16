#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Asteroid.hpp"
#include "Remnant.hpp"

#include <memory>
#include <vector>
#include <cstdlib>
#include <algorithm>

/**
* Space game controller.
*/
class Game final
{
public:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    ~Game();

    // Initialize the game and start the game loop.
    void run();

private:
    enum class GameState { Start, Running, Over };

    // Screen constants
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    const glm::vec2 SCR_SIZE = glm::vec2(SCR_WIDTH, SCR_HEIGHT);
    const glm::vec2 SCR_CENTER = SCR_SIZE / 2.0f;

    // Time constants
    const float UPDATES_PER_SEC = 60.0f;
    const float UPDATE_INTERVAL = 1.0f / UPDATES_PER_SEC;
    const double TIME_BETWEEN_STATES = 1.0;

    // Asteroid constants
    const std::size_t ASTEROID_MIN_COUNT = 5;
    const float ASTEROID_MIN_ROT_SPEED = -30.0f;
    const float ASTEROID_MAX_ROT_SPEED = 30.0f;
    const float ASTEROID_MIN_SPEED = 70.0f;
    const float ASTEROID_MAX_SPEED = 200.0f;
    const float ASTEROID_MIN_ANGLE = 20.0f;
    const float ASTEROID_MAX_ANGLE = 40.0f;
    const float ASTEROID_SIZE = 40.0f;

    // Player constants
    const glm::vec2 PLAYER_SIZE = glm::vec2(28.0f, 35.0f);
    const float PLAYER_FORCE = 400.0f;
    const float PLAYER_DECAY = 0.99f;
    const float PLAYER_TURN_SPEED = 225.0f;
    const float PLAYER_RELOAD_TIME = 0.3f;

    // Bullet constants
    const float BULLET_SPEED = 400.0f;
    const glm::vec2 BULLET_SIZE = glm::vec2(3.0f, 10.0f);
    const float BULLET_RANGE = std::min(SCR_SIZE.x, SCR_SIZE.y);
    const double BULLET_LIFETIME = BULLET_RANGE / BULLET_SPEED;

    // Level icon constants
    const glm::vec2 LEVEL_ICON_SIZE = glm::vec2(16.0f, 20.0f);
    const glm::vec3 LEVEL_ICON_COLOR = glm::vec3(0.5f, 0.5f, 0.5f);
    const float LEVEL_ICON_OFFSET = 20.0f;
    const std::size_t LEVEL_ICONS_IN_ROW = 5;

    // Remnant constants
    const std::size_t REMNANT_COUNT = 10;
    const glm::vec2 REMNANT_SIZE = glm::vec2(3.0f);
    const float REMNANT_LIFETIME = 0.5f;
    const float REMNANT_MIN_SPEED = 40.0f;
    const float REMNANT_MAX_SPEED = 80.0f;

    std::unique_ptr<Window> m_window;
    std::size_t m_level;    // Current level
    GameState m_state;      // Current state
    Timer m_stateTimer;     // Timer for delay between states
    Renderer m_renderer;
    Player m_player;
    std::vector<Asteroid> m_asteroids;
    std::vector<Bullet> m_bullets;
    std::vector<Remnant> m_remnants;

    // Initialization
    void init();
    void createWindow();
    void loadResources() const;
    void setCommonUniforms() const;
    void createPlayer();
    void restartGame();             // Set the game to the state of level one

    // Game loop
    void gameLoop();
    void determineState();          // Determines the next state of the game using m_stateTimer
    void processInput();
    void shootBullet();
    void update(float deltaTime);
    void handleCollisions();
    void createRemnants(const Asteroid& asteroid);
    void handleStrayObjects();
    void rolloverObject(GameObject& gameObject); // "Rollover" the object to the other side of screen
    void render() const;
    void renderLevelCount() const;

    // State change
    void gameOver();
    void increaseLevel();       // Increase level and spawn new asteroids
    void spawnAsteroids();      // Spawn asteroids according to current level
    void createAsteroid();      // Create a new asteroid and places it randomly outside the screen
    glm::vec2 getAsteroidRandomPos(float size) const; // Get a random position of an asteroid to be created

    // Remove all objects from vector satisfying the given condition.
    template<typename T, typename F>
    void removeObjectsIf(std::vector<T>& objects, F function);
};

template<typename T, typename F>
void Game::removeObjectsIf(std::vector<T>& objects, F function)
{
    objects.erase(std::remove_if(objects.begin(), objects.end(), function), objects.end());
}

#endif