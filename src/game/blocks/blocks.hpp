#ifndef _BLOCKS_HPP_
#define _BLOCKS_HPP_

#include "game/blocks/block.hpp"

namespace game::blocks {
#define BLOCK(className) \
  class className final : public Block { \
  public: \
    const char *texture() const noexcept override; \
  };

BLOCK(DefaultBlock)
BLOCK(GrassBlock)
BLOCK(EarthBlock)
BLOCK(StoneBlock)
}

#endif
