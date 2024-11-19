#include "core/types.hpp"
#include "game/world/generators/plain_world_generator.hpp"

#include <gtest/gtest.h>

using namespace game;

TEST(PlainWorldGenerator, EmptyWorldGenerationTest)
{
  const auto kWorld = PlainWorldGenerator({})();
  ASSERT_EQ(kWorld->size(), 0);
}

TEST(PlainWorldGenerator, World_1x100_GenerationTest)
{
  PlainWorldGeneratorSettings settings;
  settings.w = 100;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 1});

  const auto kWorld = PlainWorldGenerator(settings)();

  ASSERT_EQ(kWorld->w(), 100);
  ASSERT_EQ(kWorld->h(), 1);
  ASSERT_EQ(kWorld->size(), 100);

  for (byte i = 0; i < 100; i++)
  {
    ASSERT_EQ(kWorld->at(i)->x, i);
    ASSERT_EQ(kWorld->at(i)->y, 0);
  }
}

TEST(PlainWorldGenerator, World_100x1_GenerationTest)
{
  PlainWorldGeneratorSettings settings;
  settings.w      = 1;
  settings.layers = {{blocks::DEFAULT_BLOCK, 100}};

  const auto kWorld = PlainWorldGenerator(settings)();

  ASSERT_EQ(kWorld->w(), 1);
  ASSERT_EQ(kWorld->h(), 100);
  ASSERT_EQ(kWorld->size(), 100);

  for (byte i = 0; i < 100; i++)
  {
    ASSERT_EQ(kWorld->at(i)->x, 0);
    ASSERT_EQ(kWorld->at(i)->y, i);
  }
}

TEST(PlainWorldGenerator, World_100x100_GenerationTest)
{
  PlainWorldGeneratorSettings settings;
  settings.w      = 100;
  settings.layers = {{blocks::DEFAULT_BLOCK, 100}};

  const auto kWorld = PlainWorldGenerator(settings)();

  ASSERT_EQ(kWorld->w(), 100);
  ASSERT_EQ(kWorld->h(), 100);
  ASSERT_EQ(kWorld->size(), 100 * 100);

  for (byte y = 0; y < 100; y++)
  {
    for (byte x = 0; x < 100; x++)
    {
      ASSERT_EQ(kWorld->at(x, y)->x, x);
      ASSERT_EQ(kWorld->at(x, y)->y, y);
    }
  }
}

TEST(PlainWorldGenerator, BlockLayersTest)
{
  PlainWorldGeneratorSettings settings;
  settings.w      = 1;
  settings.layers = {
    // clang-format off
    {blocks::EARTH_BLOCK, 0},
    {blocks::DEFAULT_BLOCK, 3},
    {blocks::EARTH_BLOCK, 1},
    {blocks::GRASS_BLOCK, 8},
    {blocks::DEFAULT_BLOCK, 1},
    {blocks::DEFAULT_BLOCK, 0},
    {blocks::DEFAULT_BLOCK, 0},
    {blocks::GRASS_BLOCK, 2},
    {blocks::EARTH_BLOCK, 0},
    {blocks::DEFAULT_BLOCK, 0},
    {blocks::GRASS_BLOCK, 5},
    {blocks::DEFAULT_BLOCK, 10}
    // clang-format on
  };

  const auto kWorld = PlainWorldGenerator(settings)();

  constexpr blocks::BlockId EXPECTED_BLOCKS[] = {
    // clang-format off
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK
    // clang-format on
  };

  for (ushort i = 0; i < settings.h(); i++)
  {
    ASSERT_EQ(kWorld->at(settings.h() - i - 1)->id, EXPECTED_BLOCKS[i]);
  }
}
