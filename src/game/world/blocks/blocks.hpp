#ifndef _EFE_BLOCKS_HPP_
#define _EFE_BLOCKS_HPP_

#include "game/world/blocks/block.hpp"

namespace game::blocks
{
#define BLOCK(className) \
  class className final : public Block \
  { \
  public: \
    BlockId id() const noexcept override; \
    const char *texture_path() const noexcept override; \
    float durability() const noexcept override; \
  };

BLOCK(DefaultBlock)
BLOCK(GrassBlock)
BLOCK(EarthBlock)
BLOCK(StoneBlock)
BLOCK(OakBlock)
BLOCK(OakLeaves)
BLOCK(Grass)
BLOCK(Water)
BLOCK(WaterWave)
}

#endif
