#include "Bullet.hpp"

#include <glm/vec2.hpp>

Bullet::Bullet() : velocity(0.0f), m_distanceLeft(0.0f)
{
}

void Bullet::update(float deltaTime)
{
    glm::vec2 shift = velocity * deltaTime;
    if (m_distanceLeft >= 0.0f)
    {
        m_distanceLeft -= glm::length(shift);
    }
    position += velocity * deltaTime;
}

void Bullet::setRange(float range)
{
    m_distanceLeft = range;
}

bool Bullet::isOutsideRange() const
{
    return m_distanceLeft < 0.0f;
}