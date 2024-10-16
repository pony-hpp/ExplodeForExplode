#include "game/blocks/block_id.hpp"

#define REGISTER_BLOCK_ID(v) \
  case v: return #v; break;

using namespace game::blocks;

const char *game::blocks::id_to_str(BlockId id) noexcept {
  switch (id) {
    REGISTER_BLOCK_ID(DEFAULT_BLOCK)
    REGISTER_BLOCK_ID(GRASS_BLOCK)
    REGISTER_BLOCK_ID(EARTH_BLOCK)
    REGISTER_BLOCK_ID(STONE_BLOCK)
  default: return ""; break;
  }
}
