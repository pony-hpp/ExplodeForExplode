#include "core/types.hpp"
#include "game/world/generators/plain_world_generator.hpp"

#include <gtest/gtest.h>

using namespace game;

TEST(PlainWorldGenerator, OverrideBlockInsideWorldTest)
{
  PlainWorldGeneratorSettings settings;
  settings.w = 8;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 8});
  settings.overridenBlocks = {
    // clang-format off
    {blocks::STONE_BLOCK, 7, 3},
    {blocks::EARTH_BLOCK, 1, 0}
    // clang-format on
  };

  const auto kWorld = PlainWorldGenerator(settings)();

  for (byte y = 0; y < 8; y++)
  {
    for (byte x = 0; x < 8; x++)
    {
      const blocks::BlockId kId = kWorld->at(x, y)->id;

      if (x == 7 && y == 3)
      {
        ASSERT_EQ(kId, blocks::STONE_BLOCK);
      }
      else if (x == 1 && y == 0)
      {
        ASSERT_EQ(kId, blocks::EARTH_BLOCK);
      }
      else
      {
        ASSERT_EQ(kId, blocks::DEFAULT_BLOCK);
      }
    }
  }
}

TEST(PlainWorldGenerator, OverrideBlockOutsideWorldTest)
{
  PlainWorldGeneratorSettings settings;
  settings.w = 2;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 2});
  settings.overridenBlocks = {
    // clang-format off
    {blocks::STONE_BLOCK, -5, 12},
    {blocks::EARTH_BLOCK, 3, 1},
    {blocks::DEFAULT_BLOCK, 20, -22},
    {blocks::STONE_BLOCK, 20, -22}
    // clang-format on
  };

  const auto kWorld = PlainWorldGenerator(settings)();

  for (byte y = 0; y < 2; y++)
  {
    for (byte x = 0; x < 2; x++)
    {
      ASSERT_EQ(kWorld->at(x, y)->id, blocks::DEFAULT_BLOCK);
    }
  }

  ASSERT_EQ(*kWorld->at(-5, 12), settings.overridenBlocks[0]);
  ASSERT_EQ(*kWorld->at(3, 1), settings.overridenBlocks[1]);
  ASSERT_EQ(*kWorld->at(20, -22), settings.overridenBlocks[3]);

  // Ignore `settings.overridenBlocks[2]` (replaced by
  // `settings.overridenBlocks[3]`)
  ASSERT_EQ(kWorld->at(4 + 3), nullptr);
}
