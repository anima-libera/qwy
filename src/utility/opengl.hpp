
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header opengl.hpp - Functions retalted to OepnGL (like error checking, and stuff) */

#ifndef __qwy_opengl_header__
#define __qwy_opengl_header__

#include <GL/glew.h>
#include <string>

namespace qwy
{

	/* Just return the name associated to an OpenGL symbolic error constant */
	std::string const& opengl_error_string(GLenum error);

	/* Check for OpenGL errors, output the prefix and the error flag if there is an error, dose nothing otherwise */
	void check_opengl_errors(std::string const& prefix);

}

/* DEBUG_OPENGL_ERRORS is the debug mode only version of qwy::check_opengl_errors */
#ifdef DEBUG
	#define DEBUG_OPENGL_ERRORS(prefix) qwy::check_opengl_errors(prefix)
#else
	#define DEBUG_OPENGL_ERRORS(prefix) 
#endif

#endif /* __qwy_opengl_header__ */
