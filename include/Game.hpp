#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "ResourceManager.hpp"

#include <optional>

class Game
{
public:
    Game();

    void run();

private:
    const size_t SCR_WIDTH = 800;
    const size_t SCR_HEIGHT = 600;

    std::optional<Window> m_window;
    ResourceManager m_resources;
    
    void init();
};

#endif