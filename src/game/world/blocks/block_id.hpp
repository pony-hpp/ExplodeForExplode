#ifndef EFE_BLOCK_ID_HPP
#define EFE_BLOCK_ID_HPP

#include "core/types.hpp"

namespace game::blocks
{
enum BlockId : ubyte
{
  DEFAULT_BLOCK,
  GRASS_BLOCK,
  EARTH_BLOCK,
  STONE_BLOCK,
  OAK_BLOCK,
  OAK_LEAVES,
  GRASS,
  WATER,
  WATER_WAVE
};

constexpr const char *id_to_str(BlockId id)
{
  switch (id)
  {
  case DEFAULT_BLOCK: return "Default block"; break;
  case GRASS_BLOCK: return "Grass block"; break;
  case EARTH_BLOCK: return "Earth"; break;
  case STONE_BLOCK: return "Stone"; break;
  case OAK_BLOCK: return "Oak"; break;
  case OAK_LEAVES: return "Oak leaves"; break;
  case GRASS: return "Grass"; break;
  case WATER: return "Water"; break;
  case WATER_WAVE: return "Water wave"; break;
  default: return "";
  }
}
}

#endif
