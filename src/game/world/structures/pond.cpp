#include "core/rand.hpp"
#include "core/types.hpp"
#include "game/world/structures/structures.hpp"

#include <vector>

static constexpr byte MAX_SIZE    = 20;
static constexpr byte RAND_FACTOR = 3;

namespace game::structures
{
const char *Pond::name() const noexcept { return "Pond"; }

Structure::Data Pond::data() const noexcept
{
  std::vector<BlockData> data;

  const byte kWidth = core::Rand::next(MAX_SIZE - RAND_FACTOR, MAX_SIZE);

  byte curMinW = 0, curMaxW = kWidth;
  ubyte curY = 0;
  for (ubyte i = curMinW; i < curMaxW; i++)
  {
    data.push_back({curY == 0 ? blocks::WATER_WAVE : blocks::WATER, i, -curY});

    if (i == curMaxW - 1)
    {
      curMaxW -= core::Rand::next(0, RAND_FACTOR);
      i       = curMinW + core::Rand::next(0, RAND_FACTOR);
      curMinW = i;
      curY++;

      if (curMaxW - curMinW <= RAND_FACTOR)
      {
        continue;
      }
    }
  }

  return Data(data, kWidth, curY);
}
}
