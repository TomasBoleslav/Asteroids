#include "Player.hpp"

#include "Input.hpp"
#include "Geometry.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Player::Player() : velocity(0.0f), force(0.0f), turnSpeed(0.0f), decay(0.0f), reloadTime(0.0),
m_angularVelocity(0.0f), m_userForce(0.0f)
{
}

void Player::processInput()
{
    if (Input::isKeyPressed(GLFW_KEY_LEFT))
    {
        m_angularVelocity = -turnSpeed;
    }
    if (Input::isKeyPressed(GLFW_KEY_RIGHT))
    {
        m_angularVelocity = turnSpeed;
    }
    if (Input::isKeyPressed(GLFW_KEY_UP))
    {
        m_userForce = force;
    }
}

void Player::update(float deltaTime)
{
    rotation += m_angularVelocity * deltaTime;
    rotation = geom::clampAngle(rotation);
    glm::vec2 acceleration = geom::getDirection(rotation - 90.0f) * m_userForce;
    velocity += acceleration * deltaTime;
    velocity *= decay;
    position += velocity * deltaTime;
    m_angularVelocity = 0.0f;
    m_userForce = 0.0f;
}

bool Player::canShoot() const
{
    return m_reloadTimer.finished();
}

std::shared_ptr<Bullet> Player::shoot(glm::vec2 bulletSize, float speed, double lifetime)
{
    auto bullet = std::make_shared<Bullet>();
    bullet->position = getBulletPosition(bulletSize);
    glm::vec2 bulletDir = geom::getDirection(rotation - 90.0f);
    bullet->velocity = (speed + glm::length(velocity)) * bulletDir;
    bullet->size = bulletSize;
    bullet->rotation = rotation;
    bullet->setLifetime(lifetime);
    bullet->bounds = {
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
    };
    m_reloadTimer.start(reloadTime);
    return bullet;
}

glm::vec2 Player::getBulletPosition(glm::vec2 bulletSize) const
{
    glm::vec2 normalizedBowPos = glm::vec2(0.5f, 0.0f);
    glm::mat4 playerModel = geom::getModelMatrix(position, size, rotation);
    glm::vec2 bowPos = playerModel * glm::vec4(normalizedBowPos, 0.0f, 1.0f);
    glm::vec2 centeredBulletPos = bowPos - bulletSize / 2.0f;
    return centeredBulletPos;
}