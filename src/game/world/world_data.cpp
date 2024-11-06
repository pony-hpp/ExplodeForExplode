#include "game/world/world_data.hpp"

namespace game
{
WorldData::WorldData(
  unsigned w, unsigned short h, unsigned short extraBlocks
) noexcept
  : w(w), h(h), kExtraBlocks(extraBlocks)
{
  _data = std::make_unique<BlockData[]>(w * h + extraBlocks);
}

const BlockData &WorldData::at(unsigned long long idx) const noexcept
{
  return _data[idx];
}

BlockData &WorldData::at(unsigned long long idx) noexcept { return _data[idx]; }
}
