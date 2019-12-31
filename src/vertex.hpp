
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header vertex.hpp - Gives types and tools to deal with vertices and OpenGL */

#ifndef __qwy_vertex_header__
#define __qwy_vertex_header__

#include <GL/glew.h>

namespace qwy
{


	struct Color
	{
		static qwy::Color random();

		Color(): r(1.0f), g(1.0f), b(1.0f) {}
		Color(GLfloat grey): r(grey), g(grey), b(grey) {}
		Color(GLfloat r, GLfloat g, GLfloat b): r(r), g(g), b(b) {}

		qwy::Color randomized(float amplitude) const;

		GLfloat r, g, b;
	};

	struct Coords
	{
		Coords(): x(0.0f), y(0.0f), z(0.0f) {}
		Coords(GLfloat x, GLfloat y, GLfloat z): x(x), y(y), z(z) {}

		GLfloat x, y, z;
	};

	struct TextureCoords
	{
		TextureCoords(): x(0.0f), y(0.0f) {}
		TextureCoords(GLfloat x, GLfloat y): x(x), y(y) {}

		GLfloat x, y;
	};

	struct Vector
	{
		Vector(): x(0.0f), y(0.0f), z(0.0f) {}
		Vector(GLfloat x, GLfloat y, GLfloat z): x(x), y(y), z(z) {}

		void normalize();
		qwy::Vector normalized() const;

		GLfloat x, y, z;
	};

	
	struct Vertex
	{
		Vertex() {}
		Vertex(qwy::Coords const& coords):
			coords(coords) {}
		Vertex(qwy::Coords const& coords, qwy::Color const& color):
			coords(coords), color(color) {}
		Vertex(qwy::Coords const& coords, qwy::TextureCoords const& texture_coords): 
			coords(coords), texture_coords(texture_coords) {}
		Vertex(qwy::Coords const& coords, qwy::Color const& color, qwy::TextureCoords const& texture_coords): 
			coords(coords), color(color), texture_coords(texture_coords) {}

		qwy::Coords        coords;
		qwy::Color         color;
		qwy::TextureCoords texture_coords;
		qwy::Vector        normal;
	};
	


}

#endif /* __qwy_vertex_header__ */
