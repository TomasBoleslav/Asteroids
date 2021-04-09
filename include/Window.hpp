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
    Window() = delete;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete; // NOTE: default constructor deleted and move returns object to default state 

    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    bool isKeyPressed(std::size_t key) const;
    void swapBuffers() const;
    void setClose() const;
    bool shouldClose() const;

    static void setHint(int hint, int value);

private:
    static const std::size_t KEY_COUNT = 1024;
    static std::array<bool, KEY_COUNT> s_isKeyPressed;

    GLFWwindow* m_window;

    static void initKeyArray();
    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};


#endif