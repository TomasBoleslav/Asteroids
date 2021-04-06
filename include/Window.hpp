#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

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

    void swapBuffers();

    inline unsigned int getWidth() { return m_width; }
    inline unsigned int getHeight() { return m_height; }
    int getKeyState(int key);

    void close();
    bool shouldClose();


    static void setHint(int hint, int value);

private:
    GLFWwindow* m_window;
    unsigned int m_width;
    unsigned int m_height;
};


#endif