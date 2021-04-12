#include "GameObject.hpp"

#include "Renderer.hpp"
#include "Geometry.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject() : position(0.0f), size(0.0f), color(1.0f), rotation(0.0f)
{
}

GameObject::~GameObject()
{
}

void GameObject::draw(const Renderer& renderer) const
{
    renderer.drawQuad(texture, position, size, rotation, color);
}

bool GameObject::collidesWith(const std::shared_ptr<GameObject>& other) const
{
    auto bounds1 = applyModelOnBounds();
    auto bounds2 = other->applyModelOnBounds();
    return geom::doPolygonsIntersect(bounds1, bounds2);
}

std::vector<glm::vec2> GameObject::applyModelOnBounds() const
{
    std::vector<glm::vec2> transformedBounds;
    transformedBounds.reserve(bounds.size());
    glm::mat4 model = geom::getModelMatrix(position, size, rotation);
    for (const auto& point : bounds)
    {
        glm::vec4 appliedModel = model * glm::vec4(point, 0.0, 1.0);
        glm::vec2 transformedPoint = glm::vec2(appliedModel);
        transformedBounds.push_back(transformedPoint);
    }
    return transformedBounds;
}