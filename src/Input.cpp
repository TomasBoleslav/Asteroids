#include "Input.hpp"

#include <string>
#include <stdexcept>

std::array<bool, Input::KEY_COUNT> Input::s_isKeyPressed;

void Input::reset()
{
    for (std::size_t i = 0; i < KEY_COUNT; i++)
    {
        s_isKeyPressed[i] = false;
    }
}

bool Input::isKeyPressed(std::size_t key)
{
    if (key >= KEY_COUNT)
    {
        throw std::logic_error("Key code '" + std::to_string(key) + "' is not valid.");
    }
    return s_isKeyPressed[key];
}