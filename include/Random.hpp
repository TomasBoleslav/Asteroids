#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <glm/vec2.hpp>

#include <vector>

namespace random 
{
    void setSeed(unsigned int seed);
    float getFloat();
    float getFloat(float value1, float value2);
    glm::vec2 getVec2(glm::vec2 vec1, glm::vec2 vec2);
    std::size_t getSizeT(std::size_t max);  // TODO: rename to getIndex, return maximum value size - 1

    template<typename T>
    T choose(T value1, T value2);

    template<typename T>
    T chooseFromVector(const std::vector<T>& vector);
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

template<typename T>
T random::chooseFromVector(const std::vector<T>& vector)
{
    std::size_t index = getSizeT(vector.size() - 1);
    return vector[index];
}

#endif