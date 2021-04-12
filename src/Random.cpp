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

glm::vec2 random::getVec2(glm::vec2 vec1, glm::vec2 vec2)
{
	return glm::vec2(
		getFloat(vec1.x, vec2.x),
		getFloat(vec1.y, vec2.y)
	);
}

std::size_t random::getSizeT(std::size_t max)
{
	float randomFloat = getFloat();
	if (randomFloat == 1.0f)
	{
		return max;
	}
	return static_cast<std::size_t>(randomFloat * static_cast<float>(max + 1));
}

template<typename T>
T random::choose(T value1, T value2)
{
	if (getSizeT(1) == 0)
	{
		return value1;
	}
	return value2;
}

template<typename T>
T random::chooseFromVector(const std::vector<T>& vector)
{
	std::size_t index = getSizeT(vector.size() - 1);
	return vector[index];
}