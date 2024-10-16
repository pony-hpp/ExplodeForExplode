#include "game/world_generators/plain_world_generator.hpp"

#include <gtest/gtest.h>

TEST(PlainWorldGenerator, GenerateEmptyWorldTest) {
  game::PlainWorldGeneratorSettings settings;
  settings.w = 0;
  settings.layers.push_back({});

  game::PlainWorldGenerator worldGen(settings);
  worldGen();
  // If there's no SIGSEGV, everything is ok.
}

TEST(PlainWorldGenerator, Generate_1x100_WorldTest) {
  game::PlainWorldGeneratorSettings settings;
  settings.w = 100;
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 1});

  game::PlainWorldGenerator worldGen(settings);
  const game::WorldData &world = worldGen();

  for (unsigned char i = 0; i < 100; i++) {
    ASSERT_EQ(world[0][i].x, i);
    ASSERT_EQ(world[0][i].y, 0);
  }
}

TEST(PlainWorldGenerator, Generate_100x1_WorldTest) {
  game::PlainWorldGeneratorSettings settings;
  settings.w = 1;
  settings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  settings.layers.push_back({game::blocks::EARTH_BLOCK, 74});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 25});

  game::PlainWorldGenerator worldGen(settings);
  const game::WorldData &world = worldGen();

  for (unsigned char i = 0; i < 100; i++) {
    ASSERT_EQ(world[i][0].x, 0);
    ASSERT_EQ(world[i][0].y, i);
  }
}

TEST(PlainWorldGenerator, Generate_100x100_WorldTest) {
  game::PlainWorldGeneratorSettings settings;
  settings.w = 100;
  settings.layers.push_back({game::blocks::EARTH_BLOCK, 5});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 65});
  settings.layers.push_back({game::blocks::GRASS_BLOCK, 30});

  game::PlainWorldGenerator worldGen(settings);
  const game::WorldData &world = worldGen();

  for (unsigned char i = 0; i < 100; i++) {
    for (unsigned char j = 0; j < 100; j++) {
      ASSERT_EQ(world[i][j].x, j);
      ASSERT_EQ(world[i][j].y, i);
    }
  }
}

TEST(PlainWorldGenerator, BlockLayersTest) {
  game::PlainWorldGeneratorSettings settings;
  settings.w = 1;
  settings.layers.push_back({game::blocks::EARTH_BLOCK, 0});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 3});
  settings.layers.push_back({game::blocks::EARTH_BLOCK, 1});
  settings.layers.push_back({game::blocks::GRASS_BLOCK, 8});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 1});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 0});
  settings.layers.push_back({game::blocks::GRASS_BLOCK, 2});
  settings.layers.push_back({game::blocks::EARTH_BLOCK, 0});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 0});
  settings.layers.push_back({game::blocks::GRASS_BLOCK, 5});
  settings.layers.push_back({game::blocks::DEFAULT_BLOCK, 10});

  game::PlainWorldGenerator worldGen(settings);
  const game::WorldData &world = worldGen();

  constexpr game::blocks::BlockId EXPECTED_BLOCKS[] = {
    // clang-format off
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::EARTH_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::GRASS_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK,
    game::blocks::DEFAULT_BLOCK
    // clang-format on
  };

  for (unsigned short i = 0; i < settings.h(); i++) {
    ASSERT_EQ(world[settings.h() - 1 - i][0].id, EXPECTED_BLOCKS[i]);
  }
}
