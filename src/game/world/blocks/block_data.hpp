#ifndef _EFE_BLOCK_DATA_HPP_
#define _EFE_BLOCK_DATA_HPP_

#include "game/world/blocks/block_id.hpp"

#include <utility>
#include <vector>

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

  // Once this block is destroyed, the child blocks are also destroyed.
  std::vector<std::pair<int, int>> childPoses = {};
};
}

#endif
