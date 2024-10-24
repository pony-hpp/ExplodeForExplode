#ifndef _WORLD_DATA_HPP_
#define _WORLD_DATA_HPP_

#include "game/world/blocks/block_data.hpp"

#include <memory>

namespace game
{
class WorldData final
{
public:
  explicit WorldData(
    unsigned w, unsigned short h, unsigned long long blockCount
  ) noexcept;

  const unsigned w;
  const unsigned short h;
  const unsigned long long kBlockCount;

  BlockData &operator[](unsigned long long idx) noexcept;
  const BlockData &operator[](unsigned long long idx) const noexcept;

private:
  std::unique_ptr<BlockData[]> _data;
};
}

#endif
