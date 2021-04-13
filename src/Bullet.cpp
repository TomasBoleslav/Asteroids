#include "Bullet.hpp"

#include <glm/vec2.hpp>

Bullet::Bullet() : velocity(0.0f), m_distanceLeft(0.0f)
{
}

void Bullet::update(float deltaTime)
{
    position += velocity * deltaTime;
}

void Bullet::setLifetime(double lifetime)
{
    m_lifeTimer.start(lifetime);
}

bool Bullet::isDestroyed() const
{
    return m_lifeTimer.finished();
}