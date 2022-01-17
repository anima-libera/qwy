
#include "vertex.hpp"

#include <cmath>
#include <chrono>
#include <random>

#ifdef DEBUG
	#include <iostream>
#endif


/* TODO: have stkyln,;yty then tytytvtvvvggvvgvgvgv */
static std::default_random_engine random_engine(std::chrono::system_clock::now().time_since_epoch().count());
static std::uniform_real_distribution<float> distribution_float{-1.0f, 1.0f};
static std::uniform_real_distribution<float> distribution_float_unit{0.0f, 1.0f};

/* Return a randomly created color */ /* static */
qwy::Color qwy::Color::random()
{
	return qwy::Color(
		distribution_float_unit(random_engine),
		distribution_float_unit(random_engine),
		distribution_float_unit(random_engine)
	);
}

/* Retunr a randomly modified copy of the color */
qwy::Color qwy::Color::randomized(float amplitude) const
{
	return qwy::Color(
		r + amplitude * distribution_float(random_engine),
		g + amplitude * distribution_float(random_engine),
		b + amplitude * distribution_float(random_engine)
	);
}


void qwy::Vector::normalize()
{
	float length = std::sqrt(x*x + y*y + z*z);

	#if DEBUG
		if (-1e-6 < length && length < 1e-6)
		{
			std::cout << "Warning: Vector being normalized while its length is " << length << std::endl;
		}
	#endif

	x /= length;
	y /= length;
	z /= length;
}

qwy::Vector qwy::Vector::normalized() const
{
	float length = std::sqrt(x*x + y*y + z*z);

	#if DEBUG
		if (-1e-6 < length && length < 1e-6)
		{
			std::cout << "Warning: Vector being normalized while its length is " << length << std::endl;
		}
	#endif

	return qwy::Vector(x/length, y/length, z/length);
}
