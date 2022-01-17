
#include "world.hpp"


qwy::World qwy::World::Current;


qwy::World::World()
{
	;
}

qwy::World::~World()
{
	;
}

/* Get a reference to a block given its coordinates */
qwy::Block& qwy::World::block(int x, int y, int z)
{
	return m_chunk_table.chunk(x / qwy::Chunk::Side, y / qwy::Chunk::Side, z / qwy::Chunk::Side).block(x, y, z);
}

