#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Texture2D.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

/**
* Represents a game object in a game scene.
*/
class GameObject
{
public:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
    float rotation;
    std::shared_ptr<Texture2D> texture;
    std::vector<glm::vec2> bounds;      // Bounds of object (polygon) used for checking collisions

    GameObject();
    virtual ~GameObject();
    void draw(const Renderer& renderer) const;
    bool collidesWith(const std::shared_ptr<GameObject>& other) const;

    // Updates the game object in real time
    virtual void update(float deltaTime) = 0;

private:
    std::vector<glm::vec2> applyModelOnBounds() const;
};

#endif