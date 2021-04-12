#include "Timer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Timer::Timer() : m_duration(0.0), m_startTime(0.0)
{
}

void Timer::start(double duration)
{
    m_duration = duration;
    m_startTime = glfwGetTime();
}

bool Timer::finished()
{
    return m_startTime + m_duration <= glfwGetTime();
}
