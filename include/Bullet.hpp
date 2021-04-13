#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"
#include "Timer.hpp"

class Bullet : public GameObject
{
public:
    glm::vec2 velocity;
    Bullet();
    void update(float deltaTime) override;
    void setLifetime(double lifetime);
    bool isDestroyed() const;

private:
    float m_distanceLeft;
    Timer m_lifeTimer;
};

#endif