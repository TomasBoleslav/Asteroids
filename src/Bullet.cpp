#include "Bullet.hpp"

#include <glm/vec2.hpp>

Bullet::Bullet() : speed(0.0f)
{
}

void Bullet::update(float deltaTime)
{
    position.y -= speed * deltaTime;
}