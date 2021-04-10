#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"

#include <memory>

class Game final
{
public:
    Game();
    ~Game();

    void run();

private:
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;
    static const double UPDATES_PER_SEC;
    static const double UPDATE_INTERVAL;

    std::unique_ptr<Window> m_window;

    ResourceManager m_resources;
    Renderer renderer;

    void init();
    void createWindow();
    void loadResources();
    void gameLoop();
    void processInput();
    void update(double dt);
    void render();
};

#endif