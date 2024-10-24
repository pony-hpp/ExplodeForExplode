#ifndef _BLOCKS_HPP_
#define _BLOCKS_HPP_

#include "game/world/blocks/block.hpp"

namespace game::blocks
{
#define BLOCK(className) \
  class className final : public Block \
  { \
  public: \
    const char *texture_path() const noexcept override; \
  };

BLOCK(DefaultBlock)
BLOCK(GrassBlock)
BLOCK(EarthBlock)
BLOCK(StoneBlock)
BLOCK(OakBlock)
BLOCK(OakLeaves)
}

#endif
