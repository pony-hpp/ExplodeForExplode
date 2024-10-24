#include "game/world/blocks/block_id.hpp"

using namespace game::blocks;

#define REGISTER_BLOCK_ID(v) \
  case v: return #v; break;

const char *game::blocks::id_to_str(BlockId id) noexcept
{
  switch (id)
  {
    REGISTER_BLOCK_ID(DEFAULT_BLOCK)
    REGISTER_BLOCK_ID(GRASS_BLOCK)
    REGISTER_BLOCK_ID(EARTH_BLOCK)
    REGISTER_BLOCK_ID(STONE_BLOCK)
    REGISTER_BLOCK_ID(OAK_BLOCK)
    REGISTER_BLOCK_ID(OAK_LEAVES)
  default: return "";
  }
}
