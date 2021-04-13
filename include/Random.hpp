#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <glm/vec2.hpp>

#include <vector>

/**
* Contains functions for easier random number generation.
*/
namespace random 
{
    // Set seed of random number generator.
    // This function should be called to initialize the random namespace.
    // If the given seed is zero then the seed is set randomly.
    void setSeed(unsigned int seed);

    // Get float in the interval [0, 1].
    float getFloat();

    // Get float between two values.
    float getFloat(float value1, float value2);

    // Get array index from 0 to size - 1.
    std::size_t getIndex(std::size_t size);

    // Choose one of two values.
    template<typename T>
    T choose(T value1, T value2);
}

template<typename T>
T random::choose(T value1, T value2)
{
    if (getFloat() <= 0.5f)
    {
        return value1;
    }
    return value2;
}

#endif