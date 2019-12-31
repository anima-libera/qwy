
#include "texture.hpp"

#include "types.hpp"
#include "vertex.hpp"
#include "libs/stb_image.h"
#include <string>
#include <GL/glew.h>
#include <iostream>


qwy::Texture::Texture():
	m_is_loaded(false)
{
	;
}

qwy::Texture::Texture(std::string const& file_path):
	m_is_loaded(false)
{
	load_from_file(file_path);
}

qwy::Texture::~Texture()
{
	destroy();
}

void qwy::Texture::load_from_file(std::string const& file_path)
{
	if (m_is_loaded)
	{
		/* TODO: handle the error OR free the current texture and continue */
	}

	/* Load the image file content */
	int width, height, color_channel_number;
	std::string complete_file_path = std::string("res/textures/") + file_path;
	byte_t* image_byte_array = stbi_load(complete_file_path.c_str(), &width, &height, &color_channel_number, 0);

	if (image_byte_array == nullptr)
	{
		/* TODO: handle the error */
		std::cout << "Error: image file " << complete_file_path << " could not be loaded" << std::endl;
		return;
	}

	/* Create the OpenGL texture */
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	/* Give it the image data loadad above */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_byte_array);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_byte_array);

	/* Set some good parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	/* It's done :) */
	m_is_loaded = true;
}

void qwy::Texture::destroy()
{
	glDeleteTextures(1, &m_texture_id);
	m_is_loaded = false;
	m_texture_id = 0;
}

void qwy::Texture::activate(int texture_unit_number)
{
	#ifdef DEBUG
		if (texture_unit_number < 0 || 31 < texture_unit_number)
		{
			/* TODO: handle the error (texture_unit_number out of range) */
		}
	#endif

	//glActiveTexture(GL_TEXTURE0 + texture_unit_number);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
}




qwy::TextureAtelas::TextureAtelas():
	m_texture_side_pixels(0),
	m_margin_pixels(0),
	m_tile_side_pixels(0)
{
	;
}

qwy::TextureAtelas::TextureAtelas(int texture_side_pixels, int margin_pixels, int tile_side_pixels):
	m_texture_side_pixels(texture_side_pixels),
	m_margin_pixels(margin_pixels),
	m_tile_side_pixels(tile_side_pixels)
{
	;
}

void qwy::TextureAtelas::set_params(int texture_side_pixels, int margin_pixels, int tile_side_pixels)
{
	m_texture_side_pixels = texture_side_pixels;
	m_margin_pixels = margin_pixels;
	m_tile_side_pixels = tile_side_pixels;
}

GLfloat qwy::TextureAtelas::pixel_size() const
{
	return 1.f / static_cast<float>(m_texture_side_pixels);
}
GLfloat qwy::TextureAtelas::margin() const
{
	return static_cast<float>(m_margin_pixels) / static_cast<float>(m_texture_side_pixels);
}
GLfloat qwy::TextureAtelas::tile_size() const
{
	return static_cast<float>(m_tile_side_pixels) / static_cast<float>(m_texture_side_pixels);
}
GLfloat qwy::TextureAtelas::tile_offset() const
{
	return margin() * 2.f + tile_size();
}




qwy::TextureCoords qwy::TileCoords::texture_coords_corner(int x_side, int y_side) const
{
	#ifdef DEBUG
		if (m_texture_atelas_ptr == nullptr)
		{
			/* TODO: handle the error */
		}
	#endif

	return qwy::TextureCoords(
		m_texture_atelas_ptr->tile_offset() * x + m_texture_atelas_ptr->margin()
			+ m_texture_atelas_ptr->tile_size() * x_side,
		m_texture_atelas_ptr->tile_offset() * y + m_texture_atelas_ptr->margin()
			+ m_texture_atelas_ptr->tile_size() * y_side
	);
}
