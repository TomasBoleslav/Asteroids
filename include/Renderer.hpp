#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"
#include "Texture2D.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>
#include <array>

class Renderer
{
public:
    Renderer();

    // TODO: predej resource manager, kam pridas bilou texturu
    void init(); // init buffers
    void drawQuad(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture,
        glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) const;
private:
    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 texCoord;
    };

    static const unsigned int VERTEX_COUNT = 6;
    unsigned int m_quadVAO;

    std::array<Vertex, Renderer::VERTEX_COUNT> getVertices();
};


#endif