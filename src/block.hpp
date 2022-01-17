
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header block.hpp - Defines Block class and other classes about blocks and stuff */

#ifndef __qwy_block_header__
#define __qwy_block_header__

#include "vertex.hpp"
#include "types.hpp"
#include "texture.hpp"
#include <string>
#include <vector>

namespace qwy
{

	/* Enumerate the possible faces of a block */
	enum class BlockFace
	{
		PlusX,
		MinusX,
		PlusY,   Top = qwy::BlockFace::PlusY,
		MinusY,  Bottom = qwy::BlockFace::MinusY,
		PlusZ,
		MinusZ
	};

	/* Coorinates of a block in the 3D world */
	struct BlockCoords
	{
		BlockCoords(): x(0), y(0), z(0) {}
		BlockCoords(int x, int y, int z): x(x), y(y), z(z) {}

		int x, y, z;
	};


	/* A possible type of blocks */
	class BlockType
	{
	public:
		BlockType();
		BlockType(std::string const& name, bool is_viewable,
			qwy::TileCoords const& top_tile, qwy::TileCoords const& side_tile, qwy::TileCoords const& bottom_tile,
			bool is_solid);
		~BlockType();

		void set_is_viewable(bool is_viewable);
		void set_top_face_tile_coords(qwy::TileCoords const& top_tile);
		void set_side_face_tile_coords(qwy::TileCoords const& side_tile);
		void set_bottom_face_tile_coords(qwy::TileCoords const& bottom_tile);
		void set_is_solid(bool is_solid);

		qwy::TileCoords const& face_tile_coords(qwy::BlockFace face) const;
		
		bool is_viewable() const {return m_is_viewable;}
		bool is_solid() const {return m_is_solid;}

	private:
		std::string m_name;
		bool m_is_viewable;
		qwy::TileCoords m_top_face_tile_coords;
		qwy::TileCoords m_side_face_tile_coords;
		qwy::TileCoords m_bottom_face_tile_coords;
		bool m_is_solid;
	};


	/* A table of block_types */
	class BlockTypeTable
	{
	public:
		static qwy::BlockTypeTable Current;

	public:
		BlockTypeTable();

		void add_block_type(qwy::BlockType const& block_type);

		qwy::BlockType const& get_block_type(index_t block_type_index) const;
		qwy::BlockType& block_type(index_t block_type_index);
		qwy::BlockType& operator[](index_t block_type_index);

		index_t last_index() const;
		size_t length() const;

	private:
		std::vector<qwy::BlockType> m_block_type_vector;
	};


	/* A block of the 3D world */
	class Block
	{
	public:
		Block();

		void be_void_block();

		void set_type_index(index_t block_type_index);

		index_t type_index() const;

	private:
		index_t m_type_index;
	};


}

#endif /* __qwy_block_header__ */
