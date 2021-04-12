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

class Renderer
{
public:
    Renderer();
    void init(std::shared_ptr<Shader> shader);
    void drawQuad(const std::shared_ptr<Texture2D>& texture, glm::vec2 position, glm::vec2 size,
        float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f)) const;

private:
    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 texCoord;
    };
    static const unsigned int VERTEX_COUNT = 6;
    unsigned int m_quadVAO;
    std::shared_ptr<Shader> m_shader;

    std::array<Vertex, Renderer::VERTEX_COUNT> getVertices() const;
    unsigned int createQuadVAO(const void* data) const;
    std::shared_ptr<Texture2D> createWhiteTexture() const;
};


#endif