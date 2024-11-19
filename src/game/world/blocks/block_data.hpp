#ifndef _EFE_BLOCK_DATA_HPP_
#define _EFE_BLOCK_DATA_HPP_

#include "game/world/blocks/block_id.hpp"

namespace game
{
struct BlockData
{
  inline bool operator==(const BlockData &rhs) const noexcept
  {
    return rhs.id == id && rhs.x == x && rhs.y == y;
  }

  blocks::BlockId id;
  int x, y;
};
}

#endif
