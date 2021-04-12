#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"

class Bullet : public GameObject
{
public:
    glm::vec2 velocity;
    Bullet();
    void update(float deltaTime) override;
};

#endif