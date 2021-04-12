#include "Player.hpp"

#include "Input.hpp"
#include "Geometry.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

Player::Player() : shootDelay(1.0), velocity(0.0f), direction(0.0f)
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
	// TODO: do not leave window


	/*/
	glm::vec2 desiredVelocity;
	if (direction == glm::vec2(0.0f))
	{
		desiredVelocity = glm::vec2(0.0f);
	}
	else
	{
		glm::vec2 target = maxSpeed * glm::normalize(direction);
		if (currentVelocity == glm::vec2(0.0f))
		{
			desiredVelocity = target;
		}
		else
		{
			desiredVelocity = maxSpeed * glm::normalize(currentVelocity + target);
		}
	}
	currentVelocity = interpolate(currentVelocity, desiredVelocity);
	position += currentVelocity * (float)deltaTime;
	direction = glm::vec2(0.0f);
	/**/
	/**/
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
	nextShotTimer.start(shootDelay);
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

glm::vec2 Player::interpolate(glm::vec2 velocity, glm::vec2 desiredVelocity)
{
	/*
	float velocityChange = glm::distance(velocity, desiredVelocity);
	float progress = accelerationTime * glm::clamp(1 - velocityChange / maxSpeed, 0.0f, 1.0f);
	float t = accelerationFunction(progress, accelerationTime, maxSpeed);
	return t * velocity + (1 - t) * desiredVelocity;*/
	return geom::zeroVector;
}

float Player::accelerationFunction(float x, float maxDomain, float maxValue)
{
	constexpr float pi = glm::pi<float>();
	return (-glm::cos(x * pi / maxDomain) + 1) / 2;
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