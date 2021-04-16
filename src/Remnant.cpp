#include "Remnant.hpp"

#include "Geometry.hpp"

Remnant::Remnant() : velocity(0.0f), rotationSpeed(0.0f)
{
}

void Remnant::update(float deltaTime)
{
    rotation += rotationSpeed * deltaTime;
    rotation = geom::clampAngle(rotation);
    position += velocity * deltaTime;
}

void Remnant::setLifetime(double lifetime)
{
    m_lifeTimer.start(lifetime);
}

bool Remnant::isDestroyed() const
{
    return m_lifeTimer.finished();
}