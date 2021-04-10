#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObject.hpp"

#include <glm/vec2.hpp>

class Player : public GameObject
{
public:
    float forceValue;

    Player();
    //Player(std::shared_ptr<Texture2D> texture, const std::vector<glm::vec2>& bounds, float force);

    void processInput();
    virtual void update(double deltaTime) override;

private:
    glm::vec2 currentVelocity;
    glm::vec2 userForce;

    glm::vec2 applyUserForce(glm::vec2 velocity, float deltaTime);
    glm::vec2 applyDragForce(glm::vec2 velocity, float deltaTime);
    glm::vec2 computeTrajectory(glm::vec2 velocity, float deltaTime);
    glm::vec2 velocityFromForce(glm::vec2 force, float deltaTime);
};

#endif