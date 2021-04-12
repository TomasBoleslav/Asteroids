#include "Bullet.hpp"

#include <glm/vec2.hpp>

Bullet::Bullet() : velocity(0.0f)
{
}

void Bullet::update(float deltaTime)
{
    position += velocity * deltaTime;
}