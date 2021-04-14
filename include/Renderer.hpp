#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"
#include "Texture2D.hpp"
#include "ResourceManager.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>
#include <array>
#include <cstddef>

/**
* Renders quads to screen (static rendering).
*/
class Renderer final
{
public:
    Renderer();
    ~Renderer();

    // Initialize the renderer and set a shader for drawing.
    void init(const Shader& shader);

    // Draw a quad. If the given texture is not valid, a white texture will be used.
    void drawQuad(const Texture2D& texture, glm::vec2 position, glm::vec2 size,
        float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f)) const;

private:
    // Vertex data that will be sent to graphics card.
    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 texCoord;
    };
    // Number of vertices in a quad.
    static const std::size_t VERTEX_COUNT = 6;

    unsigned int m_quadVAO;     // ID of used vertex array object.
    Shader m_shader;

    std::array<Vertex, Renderer::VERTEX_COUNT> getVertices() const;
    unsigned int createQuadVAO(const void* data) const;
    Texture2D createWhiteTexture() const;
};

#endif