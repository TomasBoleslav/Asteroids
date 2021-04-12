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
    std::size_t getSizeT(std::size_t max);
    
    template<typename T>
    T choose(T value1, T value2);

    template<typename T>
    T chooseFromVector(const std::vector<T>& vector);
}

#endif