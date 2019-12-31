
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header chunk.hpp - Defines Chunk class and ChunkTable class */

#ifndef __qwy_chunk_header__
#define __qwy_chunk_header__

#include "block.hpp"
#include "mesh.hpp"
#include "vertex.hpp"

namespace qwy
{
	class World;
}

namespace qwy
{


	/* A cubic region of the 3D world */
	class Chunk
	{
	public:
		static constexpr int Side = 21;

	public:
		Chunk();
		Chunk(int x_min, int y_min, int z_min);

		/* Set the chunk position (min int coords) */
		void set_min_position(int x_min, int y_min, int z_min);

		/* Make the chunk empty */
		void set_empty();

		/* TEST in test - generate a world on the size of this chunk, in this chunk only xdd */
		void generate_blocks();

		/* Get the coords of a block in the world rather than in the chunk */
		qwy::BlockCoords get_block_coords(int cx, int cy, int cz);

		/* TEST generate a mesh out of the block array of the chunk */
		void generate_mesh();

		/* TEST draw the block mesh */
		void draw_block_mesh() const;

		/* TEST test if the block is solid */
		bool is_in_solid_block(float fx, float fy, float fz) const;

		/* Get a reference to a block given its coordinates in the world */
		qwy::Block& block(int x, int y, int z);

	private:
		int m_chunk_x_min, m_chunk_y_min, m_chunk_z_min;
		qwy::Block m_block_array[qwy::Chunk::Side][qwy::Chunk::Side][qwy::Chunk::Side];
		qwy::Mesh m_block_mesh;

	public:
		qwy::World* m_parent_world_ptr;
	};


	/* A table of chunks */
	class ChunkTable
	{
	public:
		ChunkTable();
		ChunkTable(int side_size);
		~ChunkTable();

		/* Set the size of the side of the chunk table */
		void set_side_size(int side_size);
	
		/* Get size information */
		int get_size() const;
		int get_side_size() const;
	
		/* Get a reference to a chunk specified by it's world_chunk_coodinates */
		qwy::Chunk& chunk(int wcx, int wcy, int wcz);
	
	private:
		int m_side_size;
		std::vector<qwy::Chunk> m_chunk_vector;
	};


}

#endif /* __qwy_chunk_header__ */
