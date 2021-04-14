#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Timer.hpp"

#include <glm/vec2.hpp>

/**
* Represents a player as a space ship.
*/
class Player : public GameObject
{
public:
    glm::vec2 velocity;
    float force;        // Force pushing the player forward.
    float turnSpeed;    // Turning speed in degrees per second.
    float decay;        // Decay of the player's speed.
    double reloadTime;  // Time after which the player can shoot again.

    Player();

    // Process input from keyboard and determine the next direction.
    void processInput();

    // Move the player.
    virtual void update(float deltaTime) override;

    // Check if reload time is up.
    bool canShoot();

    // Shoot a bullet in the direction of player.
    std::shared_ptr<Bullet> shoot(glm::vec2 bulletSize, float speed, double lifetime);

private:
    Timer m_reloadTimer;
    float m_angularVelocity;    // Angular velocity that was read from input.
    float m_userForce;          // Force that was read from input.

    glm::vec2 getBulletPosition(glm::vec2 bulletSize);
};

#endif