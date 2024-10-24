#include "game/world_generators/plain_world_generator.hpp"

#include <algorithm>

namespace game
{
unsigned short PlainWorldGeneratorSettings::h() const noexcept
{
  if (!_cachedH)
  {
    for (const auto &layer : layers)
    {
      _cachedH += layer.second;
    }
  }

  return _cachedH;
}

unsigned long long PlainWorldGeneratorSettings::block_count() const noexcept
{
  unsigned long long res = w * h();
  if (structures.empty())
  {
    return res;
  }

  std::vector<std::pair<int, int>> extraBlocks;
  for (const WorldStructure &worldStructure : structures)
  {
    const auto kStructure     = structureFactory(std::get<0>(worldStructure));
    const auto kStructureData = kStructure->data();

    for (unsigned short y = 0; y < kStructure->h(); y++)
    {
      for (unsigned short x = 0; x < kStructure->w(); x++)
      {
        const BlockData &kBlock = kStructureData[y * kStructure->w() + x];
        const int kX            = std::get<1>(worldStructure) + kBlock.x;
        const int kY            = std::get<2>(worldStructure) + kBlock.y;

        if (!kBlock.enabled ||
            std::find(
              extraBlocks.cbegin(), extraBlocks.cend(),
              std::pair<int, int>(kX, kY)
            ) != extraBlocks.cend() ||
            ((kY < h() && kY >= 0) && (kX < (int)w && kX >= 0)))
        {
          continue;
        }

        extraBlocks.push_back({kX, kY});
        res++;
      }
    }
  }

  return res;
}
}
