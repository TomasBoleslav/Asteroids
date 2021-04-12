#include "Random.hpp"

#include <cstdlib>
#include <ctime>

void random::setSeed(unsigned seed)
{
	if (seed == 0)
	{
		std::srand(std::time(0));
	}
	else
	{
		std::srand(seed);
	}
}

float random::getFloat()
{
	return static_cast<float>(std::rand()) / static_cast<float>(std::RAND_MAX);
}

float random::getFloat(float minValue, float maxValue)
{
	return minValue + getFloat() * (maxValue - minValue);
}

glm::vec2 random::getVec2(glm::vec2 minValue, glm::vec2 maxValue)
{
	return glm::vec2(
		getFloat(minValue.x, maxValue.x),
		getFloat(minValue.y, maxValue.y)
	);
}
