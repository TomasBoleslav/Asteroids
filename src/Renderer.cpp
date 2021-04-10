#include "Renderer.hpp"

#include "Errors.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <array>
#include <cstdint>

Renderer::Renderer() : m_quadVAO(0), m_whiteTexture(nullptr)
{
}

void Renderer::init(ResourceManager& resources)
{
    std::array<Vertex, VERTEX_COUNT> vertices = getVertices();
    m_quadVAO = createQuadVAO(vertices.data());
    m_whiteTexture = createWhiteTexture();
    resources.addTexture("white", m_whiteTexture);
}

void Renderer::drawQuad(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture, glm::vec3 color) const
{
    if (texture)
    {
        texture->bind();
    }
    else
    {
        m_whiteTexture->bind();
    }
    shader->setVec3("u_color", color);
    GL_CALL(glBindVertexArray(m_quadVAO));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT));
    GL_CALL(glBindVertexArray(0));
}

std::array<Renderer::Vertex, Renderer::VERTEX_COUNT> Renderer::getVertices() const
{
    return std::array<Vertex, VERTEX_COUNT> {
        glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f),

        glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)
    };
}

unsigned int Renderer::createQuadVAO(const void* data) const
{
    unsigned int VAO, VBO;
    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glGenBuffers(1, &VBO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(Vertex), data, GL_STATIC_DRAW));

    GL_CALL(glBindVertexArray(VAO));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, position))));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, texCoord))));
    GL_CALL(glEnableVertexAttribArray(1));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    return VAO;
}

std::shared_ptr<Texture2D> Renderer::createWhiteTexture() const
{
    Texture2D::Settings settings;
    settings.internalFormat = GL_RGBA8;
    settings.format = GL_RGBA;
    settings.wrapS = settings.wrapT = GL_LINEAR;
    settings.filterMin = settings.filterMag = GL_CLAMP_TO_EDGE;
    uint32_t color = 0xffffffff;
    return std::make_shared<Texture2D>(1, 1, &color);
}