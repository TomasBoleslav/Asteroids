#include "Asteroid.hpp"

#include "Geometry.hpp"

Asteroid::Asteroid() : velocity(0.0f), rotationSpeed(0.0f)
{
}

void Asteroid::update(float deltaTime)
{
    rotation += rotationSpeed * deltaTime;
    rotation = geom::clampAngle(rotation);
    position += velocity * deltaTime;
}