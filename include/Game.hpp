#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"

#include <optional>

class Game
{
public:
    Game();

    void run();

private:
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;

    std::optional<Window> m_window;
    ResourceManager m_resources;
    Renderer renderer;
    GameObject rectangle;

    void init();
    void createWindow();
    void loadResources();

    unsigned int createVAO();
    void drawQuad(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture,
        glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) const;
    void drawGameObject(const std::shared_ptr<Shader>& shader, const GameObject& object) const;
};

#endif