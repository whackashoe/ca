#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "color.hpp"

extern std::mt19937 rng;


Color::Color()
{
	std::uniform_int_distribution<uint32_t> uint_dist1000(0,1000);

	auto random = [&]() -> float { 
		return static_cast<float>(100.0 / static_cast<float>(uint_dist1000(rng)));
	};

	r = random();
	g = random();
	b = random();
}