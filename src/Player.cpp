#include "Player.hpp"

#include "Input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/common.hpp>

Player::Player() : forceValue(1000.0f), userForce(0.0f), currentVelocity(0.0f)
{
}

void Player::processInput()
{
	if (Input::isKeyPressed(GLFW_KEY_RIGHT))
	{
		userForce.x = forceValue;
	}
	if (Input::isKeyPressed(GLFW_KEY_LEFT))
	{
		userForce.x = -forceValue;
	}
	if (Input::isKeyPressed(GLFW_KEY_DOWN))
	{
		userForce.y = forceValue;
	}
	if (Input::isKeyPressed(GLFW_KEY_UP))
	{
		userForce.y = -forceValue;
	}
}

void Player::update(double deltaTime)
{
	// drag against new force or velocity? against velocity, there can be no force if no input from user
	if (userForce != glm::vec2(0.0f))
	{
		int test = 1;
	}
	glm::vec2 nextVelocity = applyUserForce(currentVelocity, (float)deltaTime);
	nextVelocity = applyDragForce(nextVelocity, (float)deltaTime);
	glm::vec2 shift = computeTrajectory(nextVelocity, (float)deltaTime);
	position += shift;

	currentVelocity = nextVelocity;
	userForce = glm::vec2(0.0f);
}

glm::vec2 Player::applyUserForce(glm::vec2 velocity, float deltaTime)
{
	glm::vec2 velocityFromForce = deltaTime * userForce;
	return velocity + velocityFromForce;
}

glm::vec2 Player::applyDragForce(glm::vec2 velocity, float deltaTime)
{
	float absVelocity = glm::length(velocity);
	glm::vec2 dragForce = 0.01f * absVelocity * velocity;
	glm::vec2 dragVelocity = velocityFromForce(dragForce, deltaTime);
	return velocity - dragVelocity;
}

glm::vec2 Player::computeTrajectory(glm::vec2 velocity, float deltaTime)
{
	return velocity * deltaTime;
}

glm::vec2 Player::velocityFromForce(glm::vec2 force, float deltaTime)
{
	return force * deltaTime;
}