#include "Asteroid.hpp"

#include "Geometry.hpp"

#include <glm/mat4x4.hpp>

Asteroid::Asteroid() : velocity(0.0f), rotationSpeed(0.0f)
{
}

void Asteroid::update(float deltaTime)
{
    rotation += rotationSpeed * deltaTime;
    rotation = geom::clampAngle(rotation);
    position += velocity * deltaTime;
}

glm::vec2 Asteroid::getRemnantOrigin() const
{
    glm::vec2 normalizedCenter = glm::vec2(0.5f, 0.5f);
    glm::mat4 model = geom::getModelMatrix(position, size, rotation);
    glm::vec2 centerPos = model * glm::vec4(normalizedCenter, 0.0f, 1.0f);
    return centerPos;
}
