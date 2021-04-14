#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <cstddef>

/**
* Game window using GLFW and glad libraries.
*/
class Window final
{
public:
    // Create a window, set it as current context and initialize glad.
    // Key presses will be stored in Input class.
    // Throws std::runtime_error if not succesful. 
    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    // Swap the front and back buffer.
    void swapBuffers() const;

    // Set the close flag to true.
    void setToClose() const;

    // Check the close flag.
    bool shouldClose() const;

    // Set a hint for the next window to be created.
    static void setHint(int hint, int value);

private:
    GLFWwindow* m_window;
};

#endif