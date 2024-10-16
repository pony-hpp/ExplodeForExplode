#ifndef _BLOCK_ID_HPP_
#define _BLOCK_ID_HPP_

namespace game::blocks {
enum BlockId : unsigned char {
  DEFAULT_BLOCK,
  GRASS_BLOCK,
  EARTH_BLOCK,
  STONE_BLOCK
};

extern const char *id_to_str(BlockId id) noexcept;
}

#endif
