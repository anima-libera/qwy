
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header world.hpp - Defines World class that strores what it takes to describe the world */

#ifndef __qwy_world_header__
#define __qwy_world_header__

#include "block.hpp"
#include "chunk.hpp"

namespace qwy
{


	class World
	{
	public:
		static World Current;

	public:
		World();
		~World();

		qwy::Block& block(int x, int y, int z);

	public:
		qwy::ChunkTable m_chunk_table;
	};


}

#endif /* __qwy_world_header__ */
