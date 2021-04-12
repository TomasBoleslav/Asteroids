#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <glm/vec2.hpp>

namespace random
{
    void setSeed(unsigned seed);
    float getFloat();
    float getFloat(float minValue, float maxValue);
    glm::vec2 getVec2(glm::vec2 minValue, glm::vec2 maxValue);
}

#endif