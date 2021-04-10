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

class GameObject
{
public:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
    float rotation;
    std::shared_ptr<Texture2D> texture;
    std::vector<glm::vec2> bounds;

    GameObject();
    virtual ~GameObject();
    virtual void update(double deltaTime) = 0;
    void draw(const Renderer& renderer, const std::shared_ptr<Shader>& shader) const;
    bool collidesWith(const std::shared_ptr<GameObject>& other) const;

protected:
    glm::mat4 getModelMatrix() const;
    virtual void setUniforms(const std::shared_ptr<Shader>& shader) const {};

private:
    std::vector<glm::vec2> applyModelOnBounds() const;
    bool pointInPolygon(const std::vector<glm::vec2>& polygon, glm::vec2 point) const;
};

/*
class RollingBackground final : public GameObject
{
public:
    double rollingSpeed;
    RollingBackground(glm::vec2 size, std::shared_ptr<Texture2D> texture, double rollingSpeed);
    void update(double deltaTime) override;
    void setUniforms(const std::shared_ptr<Shader>& shader) const override;
};

class Asteroid : public GameObject
{
public:
    Asteroid(glm::vec2 position, glm::vec2 velocity, float angularVelocity);
    void update(double deltaTime) override;
};

class Bullet : public GameObject
{
    Bullet(GameObject player, glm::vec2 velocity);
    void update(double deltaTime) override;
};
*/

#endif