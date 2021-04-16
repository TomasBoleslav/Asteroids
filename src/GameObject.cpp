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

bool GameObject::collidesWith(const GameObject& other) const
{
    auto bounds1 = applyModelOnBounds();
    auto bounds2 = other.applyModelOnBounds();
    return geom::polygonsIntersect(bounds1, bounds2);
}

std::vector<glm::vec2> GameObject::applyModelOnBounds() const
{
    glm::mat4 model = geom::getModelMatrix(position, size, rotation);
    return geom::transformPolygon(bounds, model);
}