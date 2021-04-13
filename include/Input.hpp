#ifndef INPUT_HPP
#define INPUT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstddef>

/**
* Stores input from the user.
*/
class Input final
{
public:
    static void reset();

    // Check if key is pressed. Throws std::logic_error if the key is not valid.
    static bool isKeyPressed(std::size_t key);

private:
    Input() {}

    static const std::size_t KEY_COUNT = 1024;
    static std::array<bool, KEY_COUNT> s_isKeyPressed;

    // Sets the keyboard input.
    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

#endif