#include "Timer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Timer::start(double duration)
{
    m_duration = duration;
    m_startTime = glfwGetTime();
}

bool Timer::finished()
{
    return m_startTime + m_duration <= glfwGetTime();
}
