#ifndef _BLOCK_DATA_HPP_
#define _BLOCK_DATA_HPP_

#include "game/world/blocks/block_id.hpp"

namespace game
{
struct BlockData
{
  blocks::BlockId id;
  int x, y;
  bool enabled = false;
};
}

#endif
