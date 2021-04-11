#include "Asteroid.hpp"

Asteroid::Asteroid() : velocity(0.0f), angularVelocity(0.0f)
{
}

void Asteroid::update(double deltaTime)
{
    rotation += angularVelocity * (float)deltaTime;
    position += velocity * (float)deltaTime;
}
