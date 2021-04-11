#include "GameObject.hpp"

#include "Renderer.hpp"
#include "Geometry.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject() : color(glm::vec3(1.0)), rotation(0.0)
{
}

GameObject::~GameObject()
{
}

void GameObject::draw(const Renderer& renderer, const std::shared_ptr<Shader>& shader) const
{
    shader->use();
    glm::mat4 model = getModelMatrix();
    shader->setMat4("u_model", model);
    setUniforms(shader);
    renderer.drawQuad(shader, texture, color);
}

bool GameObject::collidesWith(const std::shared_ptr<GameObject>& other) const
{
    auto bounds1 = applyModelOnBounds();
    auto bounds2 = other->applyModelOnBounds();
    return geom::doPolygonsIntersect(bounds1, bounds2);
    /*/
    for (auto&& point : bounds1)
    {
        if (geom::pointInPolygon(bounds2, point))
        {
            return true;
        }
    }
    for (auto&& point : bounds2)
    {
        if (geom::pointInPolygon(bounds1, point))
        {
            return true;
        }
    }
    return false;
    /**/
}

glm::mat4 GameObject::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    return model;
}

std::vector<glm::vec2> GameObject::applyModelOnBounds() const
{
    std::vector<glm::vec2> transformedBounds;
    transformedBounds.reserve(bounds.size());
    glm::mat4 model = getModelMatrix();
    for (const auto& point : bounds)
    {
        glm::vec4 appliedModel = model * glm::vec4(point, 0.0, 1.0);
        glm::vec2 transformedPoint = glm::vec2(appliedModel);
        transformedBounds.push_back(transformedPoint);
    }
    return transformedBounds;
}

bool GameObject::pointInPolygon(const std::vector<glm::vec2>& polygon, glm::vec2 point) const
{
    bool inside = false;
    float x = point.x;  // ray from (x,y) to right (x as horizontal axis)
    float y = point.y;
    glm::vec2 prev = polygon.back();
    for (auto&& curr : polygon)
    {
        if ((((curr.y <= y) && (y < prev.y)) ||
            ((prev.y <= y) && (y < curr.y))) &&                                     // prev and curr on different sides of half-plane
            (x - curr.x) * (prev.y - curr.y) < (prev.x - curr.x) * (y - curr.y))    // intersection with ray is on the right from x
        {
            inside = !inside;   // ray intersects with line segment
        }
        prev = curr;
    }
    return inside;
}