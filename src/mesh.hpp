
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header mesh.hpp - Defines the Mesh class */

#ifndef __qwy_mesh_header__
#define __qwy_mesh_header__

#include "vertex.hpp"
#include "block.hpp"
#include <GL/glew.h>
#include <vector>

namespace qwy
{


	/* Stores and handle OpenGL verticies arrays */
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void destroy();
	
		void add_vertex(qwy::Vertex const& vertex);
		void add_vertex(qwy::Coords const& coords, qwy::Color const& color);
		void add_vertex(qwy::Coords const& coords, qwy::TextureCoords const& texture_coords);
		void add_vertex(qwy::Coords const& coords, qwy::TextureCoords const& texture_coords, qwy::Vector const& normal);
		void add_vertex(qwy::Coords const& coords, qwy::Color const& color, qwy::TextureCoords const& texture_coords);
		void add_vertex(qwy::Coords const& coords, qwy::Color const& color, qwy::TextureCoords const& texture_coords,
			qwy::Vector const& normal);

		void add_block_face(qwy::BlockCoords const& block_coords, qwy::BlockFace block_face,
			qwy::BlockType const& block_type);

		void generate_buffers();
	
		void draw() const;
	
	private:
		std::vector<qwy::Coords>         m_coords_vector;
		std::vector<qwy::Color>          m_color_vector;
		std::vector<qwy::TextureCoords>  m_texture_coords_vector;
		std::vector<qwy::Vector>         m_normal_vector;
		bool   m_buffers_constructed;
		GLuint m_coords_buffer_id;
		GLuint m_color_buffer_id;
		GLuint m_texture_coords_buffer_id;
		GLuint m_normal_buffer_id;
	};


}

#endif /* __qwy_mesh_header__ */
