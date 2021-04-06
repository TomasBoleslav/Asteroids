#include "Game.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    try
    {
        Game game;
        game.run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Runtime error:" << std::endl << e.what() << std::endl;
        return -2;
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Logic error:" << std::endl << e.what() << std::endl;
        return -3;
    }
    glfwTerminate();
    return 0;
}