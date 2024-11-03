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

  std::vector<std::pair<int, int>> extraBlocksPoses;
  for (const WorldStructure &worldStructure : structures)
  {
    const auto kStructure     = structureFactory(worldStructure.id);
    const auto kStructureData = kStructure->data();

    for (unsigned short y = 0; y < kStructure->h(); y++)
    {
      for (unsigned short x = 0; x < kStructure->w(); x++)
      {
        const BlockData &kCurBlock = kStructureData[y * kStructure->w() + x];
        const int kAbsX            = worldStructure.x + kCurBlock.x;
        const int kAbsY            = worldStructure.y + kCurBlock.y;

        // Ignore:
        //   1. Empty blocks;
        //   2. Already counted blocks;
        //   3. Blocks that aren't outside of the world.
        if (!kCurBlock.enabled ||
            std::find(
              extraBlocksPoses.cbegin(), extraBlocksPoses.cend(),
              std::pair(kAbsX, kAbsY)
            ) != extraBlocksPoses.cend() ||
            ((kAbsY < h() && kAbsY >= 0) && (kAbsX < (int)w && kAbsX >= 0)))
        {
          continue;
        }

        extraBlocksPoses.push_back({kAbsX, kAbsY});
        res++;
      }
    }
  }

  return res;
}
}
