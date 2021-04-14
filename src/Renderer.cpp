#include "Renderer.hpp"

#include "Debug.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Geometry.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>

Renderer::Renderer() : m_quadVAO(0)
{
}

void Renderer::init(const Shader& shader)
{
    m_shader = shader;
    std::array<Vertex, VERTEX_COUNT> vertices = getVertices();
    m_quadVAO = createQuadVAO(vertices.data());
    if (!ResourceManager::hasTexture("white"))
    {
        Texture2D whiteTexture = createWhiteTexture();
        ResourceManager::addTexture("white", whiteTexture);
    }
}

void Renderer::drawQuad(const Texture2D& texture, glm::vec2 position, glm::vec2 size,
    float rotation, glm::vec3 color) const
{
    m_shader.use();
    glm::mat4 model = geom::getModelMatrix(position, size, rotation);
    m_shader.setMat4("u_model", model);
    m_shader.setVec3("u_color", color);
    if (texture.isValid())
    {
        texture.bind();
    }
    else
    {
        ResourceManager::getTexture("white").bind();
    }
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

Texture2D Renderer::createWhiteTexture() const
{
    Texture2D texture;
    Texture2D::Settings settings;
    settings.internalFormat = GL_RGBA8;
    settings.format = GL_RGBA;
    settings.wrapS = settings.wrapT = GL_REPEAT;
    settings.filterMin = settings.filterMag = GL_NEAREST;
    unsigned char data[4] = { 255, 255, 255, 255 };
    texture.generate(1, 1, data);
    return texture;
}