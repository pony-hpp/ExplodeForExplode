#include "game/world_generators/plain_world_generator.hpp"

#include <gtest/gtest.h>

using namespace game;

namespace _mock {
class CircleStructure final : public Structure {
public:
  unsigned short w() const noexcept {
    return 4;
  }

  unsigned short h() const noexcept {
    return 4;
  }

  std::unique_ptr<BlockData[]> data() const noexcept {
    std::unique_ptr<BlockData[]> data =
      std::make_unique<BlockData[]>(w() * h());

    _push_block(data, {blocks::STONE_BLOCK, 1, 0});
    _push_block(data, {blocks::EARTH_BLOCK, 2, 0});
    _push_block(data, {blocks::GRASS_BLOCK, 0, 1});
    _push_block(data, {blocks::GRASS_BLOCK, 0, 2});
    _push_block(data, {blocks::EARTH_BLOCK, 3, 1});
    _push_block(data, {blocks::EARTH_BLOCK, 3, 2});
    _push_block(data, {blocks::STONE_BLOCK, 1, 3});
    _push_block(data, {blocks::STONE_BLOCK, 2, 3});

    return data;
  }
};

static std::unique_ptr<Structure> circle_structure_factory(unsigned char
) noexcept {
  return std::make_unique<CircleStructure>();
}

static const char *circle_str(unsigned char) noexcept {
  return "CIRCLE";
}
}

static void _assert_extra_blocks(
  unsigned worldW, unsigned short worldH, int x, int y, int expectedVal
) noexcept {
  PlainWorldGeneratorSettings settings;
  settings.structureFactory = _mock::circle_structure_factory;
  settings.w                = worldW;

  settings.layers.push_back({blocks::DEFAULT_BLOCK, worldH});
  settings.structures.push_back({0, x, y});
  ASSERT_EQ(settings.block_count(), settings.w * settings.h() + expectedVal);
}

TEST(PlainWorldGenerator, BlockCountTest) {
  _assert_extra_blocks(20, 10, 0, 0, 0);
  _assert_extra_blocks(20, 10, 100, 100, 8);
  _assert_extra_blocks(20, 10, 100, -100, 8);
  _assert_extra_blocks(20, 10, -100, 100, 8);
  _assert_extra_blocks(20, 10, -100, -100, 8);

  _assert_extra_blocks(20, 10, 0, 10, 8);
  _assert_extra_blocks(20, 10, 0, 10 - 2, 4);
  _assert_extra_blocks(20, 10, 0, -2, 4);
  _assert_extra_blocks(20, 10, 0, -4, 8);

  _assert_extra_blocks(20, 10, 20, 0, 8);
  _assert_extra_blocks(20, 10, 20 - 2, 0, 4);
  _assert_extra_blocks(20, 10, -1, 0, 2);
  _assert_extra_blocks(20, 10, -4, 0, 8);

  _assert_extra_blocks(20, 10, 20, 10, 8);
  _assert_extra_blocks(20, 10, 20 - 2, 10 - 2, 6);
  _assert_extra_blocks(20, 10, -2, 10 - 2, 6);

  PlainWorldGeneratorSettings settings;
  settings.structureFactory = _mock::circle_structure_factory;
  settings.w                = 20;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 10});
  settings.structures.push_back({0, -2, 10 - 1});
  settings.structures.push_back({0, 0, 10 + 1});
  ASSERT_EQ(settings.block_count(), 20 * 10 + 13);
}

TEST(PlainWorldGenerator, GenerateEmptyWorldTest) {
  PlainWorldGeneratorSettings settings;
  settings.w = 0;
  settings.layers.push_back({});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

  ASSERT_EQ(world.w, 0);
  ASSERT_EQ(world.h, 0);
  ASSERT_EQ(world.kBlockCount, 0);
}

TEST(PlainWorldGenerator, Generate_1x100_WorldTest) {
  PlainWorldGeneratorSettings settings;
  settings.w = 100;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 1});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

  ASSERT_EQ(world.w, 100);
  ASSERT_EQ(world.h, 1);
  ASSERT_EQ(world.kBlockCount, 100);
  for (unsigned char i = 0; i < 100; i++) {
    ASSERT_EQ(world[i].x, i);
    ASSERT_EQ(world[i].y, 0);
  }
}

TEST(PlainWorldGenerator, Generate_100x1_WorldTest) {
  PlainWorldGeneratorSettings settings;
  settings.w = 1;
  settings.layers.push_back({blocks::GRASS_BLOCK, 1});
  settings.layers.push_back({blocks::EARTH_BLOCK, 74});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 25});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

  ASSERT_EQ(world.w, 1);
  ASSERT_EQ(world.h, 100);
  ASSERT_EQ(world.kBlockCount, 100);
  for (unsigned char i = 0; i < 100; i++) {
    ASSERT_EQ(world[i].x, 0);
    ASSERT_EQ(world[i].y, i);
  }
}

TEST(PlainWorldGenerator, Generate_100x100_WorldTest) {
  PlainWorldGeneratorSettings settings;
  settings.w = 100;
  settings.layers.push_back({blocks::EARTH_BLOCK, 5});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 65});
  settings.layers.push_back({blocks::GRASS_BLOCK, 30});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

  ASSERT_EQ(world.w, 100);
  ASSERT_EQ(world.h, 100);
  ASSERT_EQ(world.kBlockCount, 10000);
  for (unsigned char i = 0; i < 100; i++) {
    for (unsigned char j = 0; j < 100; j++) {
      ASSERT_EQ(world[i * 100 + j].x, j);
      ASSERT_EQ(world[i * 100 + j].y, i);
    }
  }
}

TEST(PlainWorldGenerator, BlockLayersTest) {
  PlainWorldGeneratorSettings settings;
  settings.w = 1;
  settings.layers.push_back({blocks::EARTH_BLOCK, 0});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 3});
  settings.layers.push_back({blocks::EARTH_BLOCK, 1});
  settings.layers.push_back({blocks::GRASS_BLOCK, 8});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 1});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 0});
  settings.layers.push_back({blocks::GRASS_BLOCK, 2});
  settings.layers.push_back({blocks::EARTH_BLOCK, 0});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 0});
  settings.layers.push_back({blocks::GRASS_BLOCK, 5});
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 10});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

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

  for (unsigned short i = 0; i < settings.h(); i++) {
    ASSERT_EQ(world[settings.h() - 1 - i].id, EXPECTED_BLOCKS[i]);
  }
}

TEST(PlainWorldGenerator, StructureGenerationInsideWorldTest) {
  PlainWorldGeneratorSettings settings;
  settings.structureFactory = _mock::circle_structure_factory;
  settings.structureIdToStr = _mock::circle_str;
  settings.w                = 4;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 4});
  settings.structures.push_back({0, 0, 0});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

  constexpr blocks::BlockId EXPECTED_BLOCKS[] = {
    // clang-format off
    blocks::DEFAULT_BLOCK,
    blocks::STONE_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::STONE_BLOCK,
    blocks::STONE_BLOCK,
    blocks::DEFAULT_BLOCK
    // clang-format on
  };

  for (unsigned char i = 0; i < 4; i++) {
    for (unsigned char j = 0; j < 4; j++) {
      ASSERT_EQ(world[i * 4 + j].id, EXPECTED_BLOCKS[i * 4 + j]);
      ASSERT_EQ(world[i * 4 + j].x, j);
      ASSERT_EQ(world[i * 4 + j].y, i);
    }
  }
}

TEST(PlainWorldGenerator, StructureGenerationOutsideWorldTest) {
  PlainWorldGeneratorSettings settings;
  settings.structureFactory = _mock::circle_structure_factory;
  settings.structureIdToStr = _mock::circle_str;
  settings.w                = 4;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 4});
  settings.structures.push_back({0, 5, 5});

  PlainWorldGenerator gen(settings);
  const WorldData &world = gen();

  for (unsigned char i = 0; i < 16; i++) {
    ASSERT_EQ(world[i].id, blocks::DEFAULT_BLOCK);
  }

  const std::unique_ptr<BlockData[]> kExpectedData =
    _mock::CircleStructure().data();

  for (unsigned char i = 0; i < 16; i++) {
    if (!kExpectedData[i].enabled) {
      continue;
    }

    ASSERT_EQ(world[16 + i].id, kExpectedData[i].id);
    ASSERT_EQ(world[16 + i].x, 5 + kExpectedData[i].x);
    ASSERT_EQ(world[16 + i].y, 5 + kExpectedData[i].y);
  }
}
