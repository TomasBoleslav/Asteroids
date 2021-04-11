#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObject.hpp"

#include <glm/vec2.hpp>

class Player : public GameObject
{
public:
    float forceValue;
    float friction;
    float bulletsPerSecond;

    Player();
    //Player(std::shared_ptr<Texture2D> texture, const std::vector<glm::vec2>& bounds, float force);

    void processInput();
    virtual void update(double deltaTime) override;

private:
    static const glm::vec2 zeroVector;
    glm::vec2 currentVelocity;
    glm::vec2 direction;

    glm::vec2 computeDragVelocity(glm::vec2 velocity, float deltaTime);
    glm::vec2 computeTrajectory(glm::vec2 velocity, float deltaTime);
    glm::vec2 velocityFromForce(glm::vec2 force, float deltaTime);

    glm::vec2 computeFrictionForce(glm::vec2 velocity);
    glm::vec2 computeDragForce(glm::vec2 velocity);

    glm::vec2 interpolate(glm::vec2 velocity, glm::vec2 desiredVelocity);
    float accelerationFunction(float x, float maxDomain, float maxValue);
};

#endif