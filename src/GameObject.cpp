#include "GameObject.hpp"
#include "Renderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

GameObject::GameObject()
    : position(glm::vec2()), size(glm::vec2()), color(glm::vec3()), rotation(0.0), texture(nullptr)
{
}

void GameObject::draw(const Renderer& renderer, const std::shared_ptr<Shader>& shader)
{
    renderer.drawQuad(shader, texture, position, size, rotation, color);
}
