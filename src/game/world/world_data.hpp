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
    unsigned w, unsigned short h, unsigned short extraBlocks
  ) noexcept;

  const unsigned w;
  const unsigned short h;
  const unsigned short kExtraBlocks;

  const BlockData &at(unsigned long long idx) const noexcept;
  BlockData &at(unsigned long long idx) noexcept;

private:
  std::unique_ptr<BlockData[]> _data;
};
}

#endif
