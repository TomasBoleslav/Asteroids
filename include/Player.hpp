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
    float turnSpeed;
    float decay;
    double reloadTime;

    Player();
    //Player(std::shared_ptr<Texture2D> texture, const std::vector<glm::vec2>& bounds, float force);

    void processInput();
    virtual void update(float deltaTime) override;
    bool canShoot();
    std::shared_ptr<Bullet> shoot(glm::vec2 bulletSize, float speed);

private:
    Timer reloadTimer;
    float angularVelocity;
    float userForce;

    glm::vec2 getBulletPosition(glm::vec2 bulletSize);
};

#endif