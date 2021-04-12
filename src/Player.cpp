#include "Player.hpp"

#include "Input.hpp"
#include "Geometry.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

Player::Player() : reloadTime(1.0), velocity(0.0f), direction(0.0f)
{
	friction = 100.0f;
	forceValue = 1000.0f + friction;
	//forceValue = 10.0f;
}

void Player::processInput()
{
	if (Input::isKeyPressed(GLFW_KEY_RIGHT))
	{
		direction.x = 1.0f;
	}
	if (Input::isKeyPressed(GLFW_KEY_LEFT))
	{
		direction.x = -1.0f;
	}
	if (Input::isKeyPressed(GLFW_KEY_DOWN))
	{
		direction.y = 1.0f;
	}
	if (Input::isKeyPressed(GLFW_KEY_UP))
	{
		direction.y = -1.0f;
	}
}

void Player::update(float deltaTime)
{
	glm::vec2 userForce = geom::zeroVector;
	if (direction != geom::zeroVector)
	{
		userForce = forceValue * glm::normalize(direction);
	}
	else if (glm::length(velocity) < 10.0f)
	{
		velocity = geom::zeroVector;
		return;
	}
	position += computeTrajectory(velocity, (float)deltaTime);
	glm::vec2 frictionForce = computeFrictionForce(velocity);
	glm::vec2 dragForce = computeDragForce(velocity);
	glm::vec2 velocityFromForces = velocityFromForce(userForce + frictionForce + dragForce, deltaTime);
	velocity += velocityFromForces;
	direction = geom::zeroVector;
	/**/
}

bool Player::canShoot()
{
	return nextShotTimer.finished();
}

std::shared_ptr<Bullet> Player::shoot()
{
	auto bullet = std::make_shared<Bullet>();
	nextShotTimer.start(reloadTime);
	return bullet;
}

glm::vec2 Player::computeTrajectory(glm::vec2 velocity, float deltaTime)
{
	return velocity * deltaTime;
}

glm::vec2 Player::velocityFromForce(glm::vec2 force, float deltaTime)
{
	return force * deltaTime;
}

glm::vec2 Player::computeFrictionForce(glm::vec2 velocity)
{
	if (velocity == geom::zeroVector)
	{
		return geom::zeroVector;
	}
	return (-friction) * glm::normalize(velocity);
}

glm::vec2 Player::computeDragForce(glm::vec2 velocity)
{
	float absVelocity = glm::length(velocity);
	return 0.01f * (-absVelocity) * velocity;
}