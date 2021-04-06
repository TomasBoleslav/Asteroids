#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <exception>
#include <stdexcept>

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Window::Window(unsigned int width, unsigned int height, const std::string& title) : m_width(width), m_height(height)
{
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw std::runtime_error("Failed to create GLFW window");
        // TODO: throw
    }
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
        // TODO: throw
    }
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

int Window::getKeyState(int key)
{
    return glfwGetKey(m_window, key);
}

void Window::close()
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void Window::setHint(int hint, int value)
{
    glfwWindowHint(hint, value);
}