#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "GameObject.hpp"

#include <glm/vec2.hpp>

class Asteroid : public GameObject
{
public:
    glm::vec2 velocity;
    float angularVelocity;

    Asteroid();
    void update(double deltaTime) override;
};

#endif