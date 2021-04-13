#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "GameObject.hpp"

#include <glm/vec2.hpp>

/**
* Represents a flying asteroid.
*/
class Asteroid : public GameObject
{
public:
    glm::vec2 velocity;
    float rotationSpeed;    // Rotation speed in degrees per second.

    Asteroid();

    // Move the asteroid.
    void update(float deltaTime) override;
};

#endif