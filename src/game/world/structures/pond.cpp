#include "core/rand.hpp"
#include "game/world/structures/structures.hpp"

#include <cmath>

static constexpr char _RAND_FACTOR = 3;

namespace game::structures
{
const char *Pond::name() const noexcept { return "Pond"; }
unsigned short Pond::w() const noexcept { return 20; }
unsigned short Pond::h() const noexcept { return w(); }

std::unique_ptr<BlockData[]> Pond::data() const noexcept
{
  auto data = std::make_unique<BlockData[]>(w() * h());

  unsigned short minW = 0, maxW = w();
  short curY = 0;
  for (unsigned short i = minW; i < maxW; i++)
  {
    if (i == maxW - 1)
    {
      maxW -= core::rand(0, _RAND_FACTOR);
      i    = minW + core::rand(0, _RAND_FACTOR);
      minW = i;
      curY--;

      if (maxW - minW <= _RAND_FACTOR)
      {
        continue;
      }
    }

    if (-curY < h())
    {
      _push_block(
        data, {curY == 0 ? blocks::WATER_WAVE : blocks::WATER, i, curY}
      );
    }
    else
    {
      break;
    }
  }

  return data;
}
}
