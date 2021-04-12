#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"

class Bullet : public GameObject
{
public:
    float speed;
    Bullet();
    void update(float deltaTime) override;
};

#endif