#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Timer.hpp"

#include <glm/vec2.hpp>

class Player : public GameObject
{
public:
    glm::vec2 velocity;
    float forceValue;
    float friction;
    double shootDelay;

    Player();
    //Player(std::shared_ptr<Texture2D> texture, const std::vector<glm::vec2>& bounds, float force);

    void processInput();
    virtual void update(float deltaTime) override;
    bool canShoot();
    std::shared_ptr<Bullet> shoot();

private:
    glm::vec2 direction;
    Timer nextShotTimer;

    glm::vec2 computeDragVelocity(glm::vec2 velocity, float deltaTime);
    glm::vec2 computeTrajectory(glm::vec2 velocity, float deltaTime);
    glm::vec2 velocityFromForce(glm::vec2 force, float deltaTime);

    glm::vec2 computeFrictionForce(glm::vec2 velocity);
    glm::vec2 computeDragForce(glm::vec2 velocity);

    glm::vec2 interpolate(glm::vec2 velocity, glm::vec2 desiredVelocity);
    float accelerationFunction(float x, float maxDomain, float maxValue);
};

#endif