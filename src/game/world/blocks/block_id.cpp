#include "game/world/blocks/block_id.hpp"

using namespace game::blocks;

const char *game::blocks::id_to_str(BlockId id) noexcept
{
  switch (id)
  {
  case DEFAULT_BLOCK: return "Default block"; break;
  case GRASS_BLOCK: return "Grass block"; break;
  case EARTH_BLOCK: return "Earth block"; break;
  case STONE_BLOCK: return "Stone"; break;
  case OAK_BLOCK: return "Oak"; break;
  case OAK_LEAVES: return "Oak leaves"; break;
  case GRASS: return "Grass"; break;
  case WATER: return "Water"; break;
  case WATER_WAVE: return "Water wave"; break;
  default: return "";
  }
}
