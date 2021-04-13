#include "Random.hpp"

#include <cstdlib>
#include <ctime>

void random::setSeed(unsigned int seed)
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
	return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

float random::getFloat(float value1, float value2)
{
	return value1 + getFloat() * (value2 - value1);
}

std::size_t random::getIndex(std::size_t max)
{
	float randomFloat = getFloat();
	if (randomFloat == 1.0f)
	{
		return max;
	}
	return static_cast<std::size_t>(randomFloat * static_cast<float>(max + 1));
}