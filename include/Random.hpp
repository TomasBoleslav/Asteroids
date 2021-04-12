#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <glm/vec2.hpp>

namespace random
{
    void setSeed(unsigned seed);
    float getFloat();
    float getFloat(float value1, float value2);
    glm::vec2 getVec2(glm::vec2 vec1, glm::vec2 vec2);
    std::size_t getSizeT(std::size_t max);
}

#endif