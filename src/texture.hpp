
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header texture.hpp - Defines Texture class to handle OpenGL texture managment and image loading */

#ifndef __qwy_texture_header__
#define __qwy_texture_header__

#include "vertex.hpp"
#include <string>
#include <GL/glew.h>

namespace qwy
{


	/* An OpenGL texture */
	class Texture
	{
	public:
		Texture();
		Texture(std::string const& file_path);
		~Texture();

		void load_from_file(std::string const& file_path);
		void destroy();

		void activate(int texture_unit_number = 0);

		GLuint id() const {return m_texture_id;}

	private:
		bool m_is_loaded;
		GLuint m_texture_id;
	};


	/* A texture atelas to store multiple tiles in one big texture */
	class TextureAtelas
	{
	public:
		TextureAtelas();
		TextureAtelas(int texture_side_pixels, int margin_pixels, int tile_side_pixels);

		void set_params(int texture_side_pixels, int margin_pixels, int tile_side_pixels);

		int texture_side_pixels() const {return m_texture_side_pixels;}
		int margin_pixels() const {return m_margin_pixels;}
		int tile_side_pixels() const {return m_tile_side_pixels;}
		GLfloat pixel_size() const;
		GLfloat margin() const;
		GLfloat tile_size() const;
		GLfloat tile_offset() const;

	public:
		qwy::Texture texture;

	private:
		int m_texture_side_pixels;
		int m_margin_pixels;
		int m_tile_side_pixels;
	};


	/* The coordinates of a tile on a tileset strored in a texture atelas */
	struct TileCoords
	{
		TileCoords(): x(0), y(0), m_texture_atelas_ptr(nullptr) {}
		TileCoords(int x, int y): x(x), y(y), m_texture_atelas_ptr(nullptr) {}
		TileCoords(int x, int y, qwy::TextureAtelas* texture_atelas_ptr):
			x(x), y(y), m_texture_atelas_ptr(texture_atelas_ptr) {}

		qwy::TextureCoords texture_coords_corner(int x_side, int y_side) const;

		int x, y;
		qwy::TextureAtelas* m_texture_atelas_ptr;
	};



}

#endif /* __qwy_texture_header__ */
