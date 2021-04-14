#include "Window.hpp"

#include "Input.hpp"
#include "Debug.hpp"

#include <stdexcept>
#include <cstddef>

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (0 <= key && static_cast<std::size_t>(key) < Input::KEY_COUNT)
    {
        if (action == GLFW_PRESS)
        {
            Input::s_isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Input::s_isKeyPressed[key] = false;
        }
    }
}

Window::Window(unsigned int width, unsigned int height, const std::string& title)
{
    Input::reset();
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD.");
    }
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
    GL_CALL(glViewport(0, 0, width, height));
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void Window::setToClose() const
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