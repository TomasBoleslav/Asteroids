#include "Asteroid.hpp"

Asteroid::Asteroid() : velocity(0.0f), angularVelocity(0.0f)
{
}

void Asteroid::update(float deltaTime)
{
    rotation += angularVelocity * deltaTime;
    while (rotation >= 360.0f)    // TODO: rotation in degrees or radians?
    {
        rotation -= 360.0f;
    }
    position += velocity * deltaTime;
}
