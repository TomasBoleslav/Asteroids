#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"
#include "Timer.hpp"

/**
* Represents a bullet shot by a player.
*/
class Bullet : public GameObject
{
public:
    glm::vec2 velocity;

    Bullet();

    // Move the bullet.
    void update(float deltaTime) override;

    // Set the duration for which the bullet will exist.
    void setLifetime(double lifetime);

    // Check if lifetime is up.
    bool isDestroyed() const;

private:
    Timer m_lifeTimer;
};

#endif