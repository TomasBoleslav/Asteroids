#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <cstddef>

class Window final
{
public:
    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    void swapBuffers() const;
    void setToClose() const;
    bool shouldClose() const;

    static void setHint(int hint, int value);

private:
    GLFWwindow* m_window;
};

#endif