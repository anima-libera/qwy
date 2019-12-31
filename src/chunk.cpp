
#include "chunk.hpp"

#include "block.hpp"
#include "mesh.hpp"
#include "vertex.hpp"
#include "world.hpp"
#include <chrono>
#include <random>


/* Chunk class */

qwy::Chunk::Chunk():
	m_chunk_x_min(0),
	m_chunk_y_min(0),
	m_chunk_z_min(0)
{
	set_empty();
}

qwy::Chunk::Chunk(int x_min, int y_min, int z_min):
	m_chunk_x_min(x_min),
	m_chunk_y_min(y_min),
	m_chunk_z_min(z_min)
{
	set_empty();
}

/* Set the chunk position (min int coords) */
void qwy::Chunk::set_min_position(int x_min, int y_min, int z_min)
{
	m_chunk_x_min = x_min;
	m_chunk_y_min = y_min;
	m_chunk_z_min = z_min;
}

/* Make the chunk empty */
void qwy::Chunk::set_empty()
{
	for (int cx = 0; cx < qwy::Chunk::Side; cx ++)
	{
		for (int cy = 0; cy < qwy::Chunk::Side; cy ++)
		{
			for (int cz = 0; cz < qwy::Chunk::Side; cz ++)
			{
				m_block_array[cx][cy][cz].be_void_block();
			}
		}
	}
}

/* TEST in test - generate a world on the size of this chunk, in this chunk only xdd */
void qwy::Chunk::generate_blocks()
{
	/* Init the random engine */
	std::default_random_engine random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution_pop_column{0, 80};
	std::uniform_int_distribution<int> distribution_column_heigh{1, 6};

	/* Generate the blocks */
	for (int cx = 0; cx < qwy::Chunk::Side; cx ++)
	{
		for (int cz = 0; cz < qwy::Chunk::Side; cz ++)
		{
			/* Coose the heigh of the terrain (cy) in cx, cz */
			int max_y = 10;
			if (distribution_pop_column(random_engine) == 0)
			{
				max_y += distribution_column_heigh(random_engine);
			}

			/* Make the terrain that heigh by fill the column with solid blocks */
			for (int cy = 0; cy < max_y; cy ++)
			{
				/* Set a solid block */
				m_block_array[cx][cy][cz].set_type_index(1);
			}
		}
	}
	/* Generation done :D */
}

/* Get the coords of a block in the world rather than in the chunk */
qwy::BlockCoords qwy::Chunk::get_block_coords(int cx, int cy, int cz)
{
	return qwy::BlockCoords(m_chunk_x_min + cx, m_chunk_y_min + cy, m_chunk_z_min + cz);
}

/* TEST generate a mesh out of the block array of the chunk */
void qwy::Chunk::generate_mesh()
{
	m_block_mesh.destroy();

	/* Put all the non-empty blocks in the block_mesh */
	for (int cx = 0; cx < qwy::Chunk::Side; cx ++)
	{
		for (int cy = 0; cy < qwy::Chunk::Side; cy ++)
		{
			for (int cz = 0; cz < qwy::Chunk::Side; cz ++)
			{
				#define GET_BLOCK_TYPE(CX, CY, CZ) \
					qwy::BlockTypeTable::Current.get_block_type(m_block_array[CX][CY][CZ].type_index())

				BlockType const& block_type = GET_BLOCK_TYPE(cx, cy, cz);
				if (block_type.is_viewable())
				{
					if (cx == qwy::Chunk::Side-1 || not GET_BLOCK_TYPE(cx+1, cy, cz).is_viewable())
					{
						m_block_mesh.add_block_face(get_block_coords(cx, cy, cz), qwy::BlockFace::PlusX, block_type);
					}
					if (cx == 0 || not GET_BLOCK_TYPE(cx-1, cy, cz).is_viewable())
					{
						m_block_mesh.add_block_face(get_block_coords(cx, cy, cz), qwy::BlockFace::MinusX, block_type);
					}
					if (cy == qwy::Chunk::Side-1 || not GET_BLOCK_TYPE(cx, cy+1, cz).is_viewable())
					{
						m_block_mesh.add_block_face(get_block_coords(cx, cy, cz), qwy::BlockFace::PlusY, block_type);
					}
					if (cy == 0 || not GET_BLOCK_TYPE(cx, cy-1, cz).is_viewable())
					{
						m_block_mesh.add_block_face(get_block_coords(cx, cy, cz), qwy::BlockFace::MinusY, block_type);
					}
					if (cz == qwy::Chunk::Side-1 || not GET_BLOCK_TYPE(cx, cy, cz+1).is_viewable())
					{
						m_block_mesh.add_block_face(get_block_coords(cx, cy, cz), qwy::BlockFace::PlusZ, block_type);
					}
					if (cz == 0 || not GET_BLOCK_TYPE(cx, cy, cz-1).is_viewable())
					{
						m_block_mesh.add_block_face(get_block_coords(cx, cy, cz), qwy::BlockFace::MinusZ, block_type);
					}
				}
			}
		}
	}

	m_block_mesh.generate_buffers();
}

/* TEST draw the block mesh */
void qwy::Chunk::draw_block_mesh() const
{
	m_block_mesh.draw();
}

/* TEST test if the block is solid */
bool qwy::Chunk::is_in_solid_block(float fx, float fy, float fz) const
{
	/* Get internal chunk coords out of real float coords */
	int cx = static_cast<int>(std::floor(fx)) - m_chunk_x_min;
	int cy = static_cast<int>(std::floor(fy)) - m_chunk_y_min;
	int cz = static_cast<int>(std::floor(fz)) - m_chunk_z_min;

	/* TEST test if we are in this chunk, if not then just return false */
	if (cx < 0||qwy::Chunk::Side <= cx || cy < 0||qwy::Chunk::Side <= cy || cz < 0||qwy::Chunk::Side <= cz)
	{
		return false;
	}

	/* Test if the block is solid, and return the awnser */
	return qwy::BlockTypeTable::Current.get_block_type(
		m_block_array[cx][cy][cz].type_index()).is_solid();
}

/* Get a reference to a block given its coordinates in the world */
qwy::Block& qwy::Chunk::block(int x, int y, int z)
{
	return m_block_array[x-m_chunk_x_min][y-m_chunk_y_min][z-m_chunk_z_min];
}



/* ChunkTable class */

qwy::ChunkTable::ChunkTable():
	m_side_size(0),
	m_chunk_vector(0)
{
	;
}

qwy::ChunkTable::ChunkTable(int side_size):
	m_side_size(side_size),
	m_chunk_vector(side_size * side_size * side_size)
{
	;
}

qwy::ChunkTable::~ChunkTable()
{
	;
}

/* Set the size of the side of the chunk table */
void qwy::ChunkTable::set_side_size(int side_size)
{
	m_side_size = side_size;
	m_chunk_vector.resize(side_size * side_size * side_size);
}

/* Get size information */
int qwy::ChunkTable::get_size() const
{
	return m_side_size * m_side_size * m_side_size;
}
int qwy::ChunkTable::get_side_size() const
{
	return m_side_size; 
}

/* Get a reference to a chunk specified by it's world_chunk_coodinates */
qwy::Chunk& qwy::ChunkTable::chunk(int wcx, int wcy, int wcz)
{
	return m_chunk_vector[(m_side_size * m_side_size) * wcz + (m_side_size) * wcy + wcx];
}
