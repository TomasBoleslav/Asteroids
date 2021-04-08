#include "Game.hpp"

#include "Vertex.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Texture2D.hpp"
#include "ResourceManager.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <optional>
#include <stdexcept>

Game::Game()
{
    Window::setHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    Window::setHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    Window::setHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    Window::setHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window.emplace(SCR_WIDTH, SCR_HEIGHT, "SpaceGame");
}

void Game::run()
{
    const size_t vertexCount = 4;
    const Vertex vertices[vertexCount] = {
        Vertex(Position(0.5f, 0.5f, 0.0f), Color(1.0f, 0.0f, 0.0f), TexCoord(1.0, 1.0)),
        Vertex(Position(0.5f, -0.5f, 0.0f), Color(0.0f, 1.0f, 0.0f), TexCoord(1.0, 0.0)),
        Vertex(Position(-0.5f, -0.5f, 0.0f), Color(1.0f, 0.0f, 0.0f), TexCoord(0.0, 0.0)),
        Vertex(Position(-0.5f,  0.5f, 0.0f), Color(0.0f, 0.0f, 1.0f), TexCoord(0.0, 1.0))
    };
    const size_t indexCount = 6;
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, color)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(struct Vertex, tex)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_resources.loadShader("simple", "res/shaders/simple.vert", "res/shaders/simple.frag");
    m_resources.loadTexture("wood", "res/images/wood.jpg");

    while (!m_window.value().shouldClose())
    {
        if (m_window.value().getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            m_window.value().close();
            break;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transform = glm::mat4(1.0f);
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        m_resources.getShader("simple")->use();
        m_resources.getShader("simple")->setMat4("transform", transform);

        m_resources.getTexture("wood")->bind();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        m_window.value().swapBuffers();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}