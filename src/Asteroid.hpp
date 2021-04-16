#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "GameObject.hpp"
#include "Remnant.hpp"

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

    // Get the point where asteroid remnants will originate from
    glm::vec2 getRemnantOrigin() const;
};

#endif