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

// TODO: create white texture so that color can be used (or not?)
Renderer::Renderer() : m_quadVAO(0)
{    
}

void Renderer::init()
{
    std::array<Vertex, VERTEX_COUNT> vertices = getVertices();
    unsigned int VBO;
    GL_CALL(glGenVertexArrays(1, &m_quadVAO));
    GL_CALL(glGenBuffers(1, &VBO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));

    GL_CALL(glBindVertexArray(m_quadVAO));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, position))));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, texCoord))));
    GL_CALL(glEnableVertexAttribArray(1));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
}

void Renderer::drawQuad(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture,
    glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) const
{
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader->setMat4("u_model", model);
    shader->setVec3("u_color", color);

    texture->bind();

    GL_CALL(glBindVertexArray(m_quadVAO));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT));
    GL_CALL(glBindVertexArray(0));
}

std::array<Renderer::Vertex, Renderer::VERTEX_COUNT> Renderer::getVertices()
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