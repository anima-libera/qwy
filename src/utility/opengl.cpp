
#include "opengl.hpp"

#include <GL/glew.h>
#include <string>
#include <iostream>


/* Just return the name associated to an OpenGL symbolic error constant */
std::string const& qwy::opengl_error_string(GLenum error)
{
	/* The constant string representations */
	static const std::string string_GL_NO_ERROR =                      "GL_NO_ERROR";
	static const std::string string_GL_INVALID_ENUM =                  "GL_INVALID_ENUM";
	static const std::string string_GL_INVALID_VALUE =                 "GL_INVALID_VALUE";
	static const std::string string_GL_INVALID_OPERATION =             "GL_INVALID_OPERATION";
	static const std::string string_GL_INVALID_FRAMEBUFFER_OPERATION = "GL_INVALID_FRAMEBUFFER_OPERATION";
	static const std::string string_GL_OUT_OF_MEMORY =                 "GL_OUT_OF_MEMORY";
	static const std::string string_not_an_error =                     "not_an_error";

	/* Return the apropriate one */
	switch (error)
	{
		case GL_NO_ERROR:                      return string_GL_NO_ERROR;
		case GL_INVALID_ENUM:                  return string_GL_INVALID_ENUM;
		case GL_INVALID_VALUE:                 return string_GL_INVALID_VALUE;
		case GL_INVALID_OPERATION:             return string_GL_INVALID_OPERATION;
		case GL_INVALID_FRAMEBUFFER_OPERATION: return string_GL_INVALID_FRAMEBUFFER_OPERATION;
		case GL_OUT_OF_MEMORY:                 return string_GL_OUT_OF_MEMORY;
		default:                               return string_not_an_error;
	}
}

/* Check for OpenGL errors, output the prefix and the error flag if there is an error, dose nothing otherwise */
void qwy::check_opengl_errors(std::string const& prefix)
{
	/* Loop over the errors */
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		/* Output the whole error flag on a single line (for readability) */
		std::cout << prefix << "OpenGL error flag:";
		do
		{
			std::cout << " " << qwy::opengl_error_string(error);
		}
		while ((error = glGetError()) != GL_NO_ERROR);
		std::cout << std::endl;
	}
}
