#include "game/world/generators/plain_world_generator.hpp"

#include <algorithm>

namespace game
{
ushort PlainWorldGeneratorSettings::h() const noexcept
{
  if (!_cachedH)
  {
    for (const BlockLayer &kLayer : layers)
    {
      _cachedH += kLayer.second;
    }
  }

  return _cachedH;
}

uint PlainWorldGeneratorSettings::extra_blocks() const noexcept
{
  uint res = 0;
  std::vector<std::pair<int, int>> extraBlockPoses;

  if (structuresGenerator)
  {
    for (const auto &kStructure : structuresGenerator->structuresData())
    {
      for (uint y = 0; y < kStructure.h(); y++)
      {
        for (uint x = 0; x < kStructure.w(); x++)
        {
          const BlockData *kBlock = kStructure.at(x, y);
          if (!kBlock)
          {
            continue;
          }

          const auto kFoundExtraBlock = std::find(
            extraBlockPoses.cbegin(), extraBlockPoses.cend(),
            std::pair(kBlock->x, kBlock->y)
          );

          if (kFoundExtraBlock != extraBlockPoses.cend() ||
              ((kBlock->x < (int)w && kBlock->y >= 0) &&
               (kBlock->y < h() && kBlock->x >= 0)))
          {
            continue;
          }

          extraBlockPoses.push_back({kBlock->x, kBlock->y});
          res++;
        }
      }
    }
  }

  for (const BlockData &kBlock : overridenBlocks)
  {
    const auto kFoundExtraBlock = std::find(
      extraBlockPoses.cbegin(), extraBlockPoses.cend(),
      std::pair(kBlock.x, kBlock.y)
    );

    if (kFoundExtraBlock != extraBlockPoses.cend() ||
        (kBlock.x < (int)w && kBlock.x >= 0) &&
          (kBlock.y < h() && kBlock.y >= 0))
    {
      continue;
    }

    extraBlockPoses.push_back({kBlock.x, kBlock.y});
    res++;
  }

  return res;
}
}
