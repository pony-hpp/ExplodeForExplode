#include "game/world/world_data.hpp"

namespace game
{
WorldData::WorldData(
  unsigned w, unsigned short h, unsigned long long blockCount
) noexcept
  : w(w), h(h), kBlockCount(blockCount)
{
  _data = std::make_unique<BlockData[]>(blockCount);
}

const BlockData &WorldData::operator[](unsigned long long idx) const noexcept
{
  return _data[idx];
}

BlockData &WorldData::operator[](unsigned long long idx) noexcept
{
  return _data[idx];
}
}
