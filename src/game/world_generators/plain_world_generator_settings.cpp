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

unsigned short PlainWorldGeneratorSettings::extra_blocks(
  const std::vector<std::unique_ptr<BlockData[]>> &structuresData
) const noexcept
{
  if (structures.empty())
  {
    return 0;
  }

  unsigned short res = 0;
  std::vector<std::pair<int, int>> extraBlockPoses;
  for (unsigned short i = 0; i < structures.size(); i++)
  {
    const auto kStructure = structureFactory(structures[i].id);

    for (unsigned short y = 0; y < kStructure->h(); y++)
    {
      for (unsigned short x = 0; x < kStructure->w(); x++)
      {
        const BlockData &kCurBlock = structuresData[i][y * kStructure->w() + x];

        const int kAbsX = structures[i].x + kCurBlock.x,
                  kAbsY = structures[i].y + kCurBlock.y;

        // Ignore:
        //   1. Empty blocks;
        //   2. Already counted blocks;
        //   3. Blocks that aren't outside of the world.
        if (!kCurBlock.enabled ||
            std::find(
              extraBlockPoses.cbegin(), extraBlockPoses.cend(),
              std::pair(kAbsX, kAbsY)
            ) != extraBlockPoses.cend() ||
            ((kAbsX < (int)w && kAbsX >= 0) && (kAbsY < h() && kAbsY >= 0)))
        {
          continue;
        }

        extraBlockPoses.push_back({kAbsX, kAbsY});
        res++;
      }
    }
  }

  return res;
}
}
