
#include "math.hpp"


/* The fundamental circle constant (circumference/diameter) (note that pi := tau/2) */
const float qwy::tau = 6.2831853071795f;

/* Convert degrees into radians */
float qwy::deg_to_rad(float degrees)
{
	return (degrees * qwy::tau) / 360.0f;
}

/* Return x, guarenteed to be between x_min and x_max */
float qwy::restrict_to_interval(float x, float x_min, float x_max)
{
	if (x < x_min)
	{
		return x_min;
	}
	else if (x > x_max)
	{
		return x_max;
	}
	else
	{
		return x;
	}
}
