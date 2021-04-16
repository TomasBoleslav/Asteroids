#ifndef REMNANT_HPP
#define REMNANT_HPP

#include "GameObject.hpp"
#include "Timer.hpp"

#include <glm/vec2.hpp>

/**
* Remnant after explosion that disappears after time.
*/
class Remnant : public GameObject
{
public:
    glm::vec2 velocity;
    float rotationSpeed;    // Rotation speed in degrees per second.

    Remnant();

    // Move and change the color of the remant.
    void update(float deltaTime) override;

    // Set the duration for which the remnant will exist.
    void setLifetime(double lifetime);

    // Check if lifetime is up.
    bool isDestroyed() const;

private:
    Timer m_lifeTimer;
};

#endif