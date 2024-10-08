#ifndef _WORLD_DATA_HPP_
#define _WORLD_DATA_HPP_

#include "game/blocks/block_data.hpp"

namespace game {
class WorldData final {
public:
  explicit WorldData(unsigned w, unsigned short h) noexcept;
  ~WorldData() noexcept;

  const unsigned w;
  const unsigned short h;

  BlockData *operator[](unsigned short y) const noexcept;

private:
  BlockData **_data;
};
}

#endif
