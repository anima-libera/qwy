
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header math.hpp - Simple math operations */

#ifndef __qwy_math_header__
#define __qwy_math_header__

namespace qwy
{


	/* The fundamental circle constant (circumference/diameter) (note that pi := tau/2) */
	extern const float tau;

	/* Convert degrees into radians */
	float deg_to_rad(float degrees);

	/* Return x, guarenteed to be between x_min and x_max */
	float restrict_to_interval(float x, float x_min, float x_max);

}

#endif /* __qwy_math_header__ */
