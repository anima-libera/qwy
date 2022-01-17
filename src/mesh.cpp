
#include "mesh.hpp"

#ifdef DEBUG
	#include <iostream>
#endif

#include "vertex.hpp"
#include "block.hpp"
#include <GL/glew.h>
#include <vector>



qwy::Mesh::Mesh():
	m_coords_vector(0),
	m_color_vector(0),
	m_texture_coords_vector(0),
	m_buffers_constructed(false)
{
	;
}

qwy::Mesh::~Mesh()
{
	destroy();
}

void qwy::Mesh::destroy()
{
	if (m_buffers_constructed)
	{
		glDeleteBuffers(1, &m_coords_buffer_id);
		glDeleteBuffers(1, &m_color_buffer_id);
		glDeleteBuffers(1, &m_texture_coords_buffer_id);
		glDeleteBuffers(1, &m_normal_buffer_id);

		m_buffers_constructed = false;
	}

	m_coords_vector.clear();
	m_color_vector.clear();
	m_texture_coords_vector.clear();
	m_normal_vector.clear();
}

void qwy::Mesh::add_vertex(qwy::Vertex const& vertex)
{
	add_vertex(vertex.coords, vertex.color, vertex.texture_coords, vertex.normal);
}

void qwy::Mesh::add_vertex(qwy::Coords const& coords, qwy::Color const& color)
{
	add_vertex(coords, color, qwy::TextureCoords(1.f, 1.f), qwy::Vector(0.0f, 1.0f, 0.0f));
}

void qwy::Mesh::add_vertex(qwy::Coords const& coords, qwy::TextureCoords const& texture_coords)
{
	add_vertex(coords, qwy::Color(1.f, 1.f, 1.f), texture_coords, qwy::Vector(0.0f, 1.0f, 0.0f));
}

void qwy::Mesh::add_vertex(qwy::Coords const& coords, qwy::TextureCoords const& texture_coords, qwy::Vector const& normal)
{
	add_vertex(coords, qwy::Color(1.f, 1.f, 1.f), texture_coords, normal);
}

void qwy::Mesh::add_vertex(qwy::Coords const& coords, qwy::Color const& color, qwy::TextureCoords const& texture_coords)
{
	add_vertex(coords, color, texture_coords, qwy::Vector(0.0f, 1.0f, 0.0f));
}

void qwy::Mesh::add_vertex(qwy::Coords const& coords, qwy::Color const& color, qwy::TextureCoords const& texture_coords,
	qwy::Vector const& normal)
{
	#ifdef DEBUG
		if (m_buffers_constructed)
		{
			/* TODO: handle the error */
		}
	#endif

	m_coords_vector.push_back(coords);
	m_color_vector.push_back(color);
	m_texture_coords_vector.push_back(texture_coords);
	m_normal_vector.push_back(normal.normalized());
}

void qwy::Mesh::add_block_face(qwy::BlockCoords const& block_coords, qwy::BlockFace block_face,
	qwy::BlockType const& block_type)
{
	#ifdef DEBUG
		if (m_buffers_constructed)
		{
			/* TODO: handle the error */
		}
	#endif

	float x = static_cast<float>(block_coords.x);
	float y = static_cast<float>(block_coords.y);
	float z = static_cast<float>(block_coords.z);

	qwy::TileCoords const& tile_coords = block_type.face_tile_coords(block_face);

	/* Add the vertices to the mesh according to the face */
	switch (block_face)
	{
		case qwy::BlockFace::PlusY:
			add_vertex(qwy::Coords(x-.5f, y+.5f, z-.5f), qwy::Color(1.0f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(0.0f, 1.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z-.5f), qwy::Color(1.0f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(0.0f, 1.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z+.5f), qwy::Color(1.0f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(0.0f, 1.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z+.5f), qwy::Color(1.0f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(0.0f, 1.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z-.5f), qwy::Color(1.0f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(0.0f, 1.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z+.5f), qwy::Color(1.0f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(0.0f, 1.0f, 0.0f));
		break;
		case qwy::BlockFace::MinusY:
			add_vertex(qwy::Coords(x+.5f, y-.5f, z+.5f), qwy::Color(0.4f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(0.0f, -1.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z-.5f), qwy::Color(0.4f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(0.0f, -1.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z-.5f), qwy::Color(0.4f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(0.0f, -1.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z+.5f), qwy::Color(0.4f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(0.0f, -1.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z-.5f), qwy::Color(0.4f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(0.0f, -1.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z+.5f), qwy::Color(0.4f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(0.0f, -1.0f, 0.0f));
		break;
		case qwy::BlockFace::PlusX:
			add_vertex(qwy::Coords(x+.5f, y+.5f, z-.5f), qwy::Color(0.9f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z-.5f), qwy::Color(0.9f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z+.5f), qwy::Color(0.9f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z-.5f), qwy::Color(0.9f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z+.5f), qwy::Color(0.9f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z+.5f), qwy::Color(0.9f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(1.0f, 0.0f, 0.0f));
		break;
		case qwy::BlockFace::MinusX:
			add_vertex(qwy::Coords(x-.5f, y-.5f, z+.5f), qwy::Color(0.7f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(-1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z-.5f), qwy::Color(0.7f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(-1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z-.5f), qwy::Color(0.7f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(-1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z+.5f), qwy::Color(0.7f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(-1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z+.5f), qwy::Color(0.7f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(-1.0f, 0.0f, 0.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z-.5f), qwy::Color(0.7f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(-1.0f, 0.0f, 0.0f));
		break;
		case qwy::BlockFace::PlusZ:
			add_vertex(qwy::Coords(x+.5f, y+.5f, z+.5f), qwy::Color(0.8f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(0.0f, 0.0f, 1.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z+.5f), qwy::Color(0.8f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(0.0f, 0.0f, 1.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z+.5f), qwy::Color(0.8f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(0.0f, 0.0f, 1.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z+.5f), qwy::Color(0.8f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(0.0f, 0.0f, 1.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z+.5f), qwy::Color(0.8f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(0.0f, 0.0f, 1.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z+.5f), qwy::Color(0.8f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(0.0f, 0.0f, 1.0f));
		break;
		case qwy::BlockFace::MinusZ:
			add_vertex(qwy::Coords(x-.5f, y-.5f, z-.5f), qwy::Color(0.6f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(0.0f, 0.0f, -1.0f));
			add_vertex(qwy::Coords(x+.5f, y-.5f, z-.5f), qwy::Color(0.6f), tile_coords.texture_coords_corner(1, 1), qwy::Vector(0.0f, 0.0f, -1.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z-.5f), qwy::Color(0.6f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(0.0f, 0.0f, -1.0f));
			add_vertex(qwy::Coords(x-.5f, y+.5f, z-.5f), qwy::Color(0.6f), tile_coords.texture_coords_corner(0, 0), qwy::Vector(0.0f, 0.0f, -1.0f));
			add_vertex(qwy::Coords(x-.5f, y-.5f, z-.5f), qwy::Color(0.6f), tile_coords.texture_coords_corner(0, 1), qwy::Vector(0.0f, 0.0f, -1.0f));
			add_vertex(qwy::Coords(x+.5f, y+.5f, z-.5f), qwy::Color(0.6f), tile_coords.texture_coords_corner(1, 0), qwy::Vector(0.0f, 0.0f, -1.0f));
		break;
	}
}

void qwy::Mesh::generate_buffers()
{
	/* Generate the buffers */
	glGenBuffers(1, &m_coords_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_coords_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, m_coords_vector.size()*sizeof(qwy::Coords), &m_coords_vector[0].x, GL_STATIC_DRAW);
	
	glGenBuffers(1, &m_color_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, m_color_vector.size()*sizeof(qwy::Color), &m_color_vector[0].r, GL_STATIC_DRAW);
	
	glGenBuffers(1, &m_texture_coords_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_texture_coords_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, m_texture_coords_vector.size()*sizeof(qwy::TextureCoords),
		&m_texture_coords_vector[0].x, GL_STATIC_DRAW);
	
	glGenBuffers(1, &m_normal_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, m_normal_vector.size()*sizeof(qwy::Vector), &m_normal_vector[0].x, GL_STATIC_DRAW);

	/* TODO: check the buffers and report an error if any */

	m_buffers_constructed = true;
}

void qwy::Mesh::draw() const
{
	#ifdef DEBUG
		if (not m_buffers_constructed)
		{
			/* TODO: handle the error */
		}
	#endif

	/* Bind the coords buffer */
	constexpr GLuint COORDS_LAYOUT = 0;
	glEnableVertexAttribArray(COORDS_LAYOUT);
	glBindBuffer(GL_ARRAY_BUFFER, m_coords_buffer_id);
	glVertexAttribPointer(
		COORDS_LAYOUT, /* attribute layout */
		3,             /* number of components per vertex */
		GL_FLOAT,      /* OpenGL type */
		GL_FALSE,      /* is normalized */
		0,             /* wants to stride */
		0              /* offset in the buffer */
	);

	/* Bind the color buffer */
	constexpr GLuint COLOR_LAYOUT = 1;
	glEnableVertexAttribArray(COLOR_LAYOUT);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer_id);
	glVertexAttribPointer(
		COLOR_LAYOUT, /* attribute layout */
		3,            /* number of components per vertex */
		GL_FLOAT,     /* OpenGL type */
		GL_FALSE,     /* is normalized */
		0,            /* wants to stride */
		0             /* offset in the buffer */
	);

	/* Bind the texture coords buffer */
	constexpr GLuint TEXTURE_COORDS_LAYOUT = 2;
	glEnableVertexAttribArray(TEXTURE_COORDS_LAYOUT);
	glBindBuffer(GL_ARRAY_BUFFER, m_texture_coords_buffer_id);
	glVertexAttribPointer(
		TEXTURE_COORDS_LAYOUT, /* attribute layout */
		2,            /* number of components per vertex */
		GL_FLOAT,     /* OpenGL type */
		GL_FALSE,     /* is normalized */
		0,            /* wants to stride */
		0             /* offset in the buffer */
	);

	/* Bind the normal buffer */
	constexpr GLuint NORMAL_LAYOUT = 3;
	glEnableVertexAttribArray(NORMAL_LAYOUT);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer_id);
	glVertexAttribPointer(
		NORMAL_LAYOUT,/* attribute layout */
		3,            /* number of components per vertex */
		GL_FLOAT,     /* OpenGL type */
		GL_FALSE,     /* is normalized */
		0,            /* wants to stride */
		0             /* offset in the buffer */
	);

	/* Draw */
	glDrawArrays(
		GL_TRIANGLES,          /* Primitive type */
		0,                     /* Offset in the buffer */
		m_coords_vector.size() /* Vertex number */
	);

	/* Clean up */
	glDisableVertexAttribArray(COORDS_LAYOUT);
	glDisableVertexAttribArray(COLOR_LAYOUT);
	glDisableVertexAttribArray(TEXTURE_COORDS_LAYOUT);
	glDisableVertexAttribArray(NORMAL_LAYOUT);
}
