#include "Asteroid.hpp"

Asteroid::Asteroid() : velocity(0.0f), angularVelocity(0.0f)
{
}

void Asteroid::update(float deltaTime)
{
    rotation += angularVelocity * deltaTime;
    position += velocity * deltaTime;
}
