#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <array>

std::array<bool, Window::KEY_COUNT> Window::s_isKeyPressed;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (0 <= key && key < Window::KEY_COUNT)
    {
        if (action == GLFW_PRESS)
        {
            Window::s_isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Window::s_isKeyPressed[key] = false;
        }
    }
}

Window::Window(unsigned int width, unsigned int height, const std::string& title)
{
    initKeyArray();
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw std::runtime_error("Failed to create GLFW window.");
        // TODO: throw
    }
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD.");
        // TODO: throw
    }
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

bool Window::isKeyPressed(std::size_t key) const
{
    if (key >= KEY_COUNT)
    {
        throw std::logic_error("Key code '" + std::to_string(key) + "' is not valid.");
    }
    return s_isKeyPressed[key];
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void Window::setClose() const
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::setHint(int hint, int value)
{
    glfwWindowHint(hint, value);
}

void Window::initKeyArray()
{
    for (std::size_t i = 0; i < KEY_COUNT; i++)
    {
        s_isKeyPressed[i] = false;
    }
}