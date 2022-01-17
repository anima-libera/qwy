
#include "block.hpp"

#include "vertex.hpp"
#include "types.hpp"
#include "texture.hpp"


/* BlockType class */

qwy::BlockType::BlockType():
	m_is_viewable(true),
	m_is_solid(true)
{
	;
}

qwy::BlockType::BlockType(std::string const& name, bool is_viewable,
	qwy::TileCoords const& top_tile, qwy::TileCoords const& side_tile, qwy::TileCoords const& bottom_tile,
	bool is_solid):

	m_name(name),
	m_is_viewable(is_viewable),
	m_top_face_tile_coords(top_tile),
	m_side_face_tile_coords(side_tile),
	m_bottom_face_tile_coords(bottom_tile),
	m_is_solid(is_solid)
{
	;
}

qwy::BlockType::~BlockType()
{
	;
}

void qwy::BlockType::set_is_viewable(bool is_viewable)
{
	m_is_viewable = is_viewable;
}
void qwy::BlockType::set_top_face_tile_coords(qwy::TileCoords const& top_tile)
{
	m_top_face_tile_coords = top_tile;
}
void qwy::BlockType::set_side_face_tile_coords(qwy::TileCoords const& side_tile)
{
	m_side_face_tile_coords = side_tile;
}
void qwy::BlockType::set_bottom_face_tile_coords(qwy::TileCoords const& bottom_tile)
{
	m_bottom_face_tile_coords = bottom_tile;
}
void qwy::BlockType::set_is_solid(bool is_solid)
{
	m_is_solid = is_solid;
}

qwy::TileCoords const& qwy::BlockType::face_tile_coords(qwy::BlockFace face) const
{
	#ifdef DEBUG
		if (not m_is_viewable)
		{
			/* TODO: handle the error (do not request that to an invisible block type) */
		}
	#endif

	if (face == qwy::BlockFace::Top)
	{
		return m_top_face_tile_coords;
	}
	else if (face == qwy::BlockFace::PlusX || face == qwy::BlockFace::MinusX
		|| face == qwy::BlockFace::PlusZ || face == qwy::BlockFace::MinusZ)
	{
		return m_side_face_tile_coords;
	}
	else /* face == qwy::BlockFace::Bottom */
	{
		return m_bottom_face_tile_coords;
	}
}



/* BlockTypeTable class */

qwy::BlockTypeTable qwy::BlockTypeTable::Current;

qwy::BlockTypeTable::BlockTypeTable():
	m_block_type_vector(0)
{
	;
}

void qwy::BlockTypeTable::add_block_type(qwy::BlockType const& block_type)
{
	m_block_type_vector.push_back(block_type);
}

qwy::BlockType const& qwy::BlockTypeTable::get_block_type(index_t block_type_index) const
{
	return m_block_type_vector[block_type_index];
}

qwy::BlockType& qwy::BlockTypeTable::block_type(index_t block_type_index)
{
	return m_block_type_vector[block_type_index];
}
qwy::BlockType& qwy::BlockTypeTable::operator[](index_t block_type_index)
{
	return m_block_type_vector[block_type_index];
}

index_t qwy::BlockTypeTable::last_index() const
{
	return m_block_type_vector.size()-1;
}

size_t qwy::BlockTypeTable::length() const
{
	return m_block_type_vector.size();
}



/* Block class */

qwy::Block::Block():
	m_type_index(0)
{
	;
}

void qwy::Block::be_void_block()
{
	m_type_index = 0;
}

void qwy::Block::set_type_index(index_t block_type_index)
{
	m_type_index = block_type_index;
}

index_t qwy::Block::type_index() const
{
	return m_type_index;
}

