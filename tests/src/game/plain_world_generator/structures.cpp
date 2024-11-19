#include "core/types.hpp"
#include "game/world/generators/plain_world_generator.hpp"

#include <gtest/gtest.h>

using namespace game;

namespace _mock
{
class CircleStructure final : public Structure
{
public:
  inline const char *name() const noexcept override { return "Circle"; }

  Data data() const noexcept override
  {
    Data data(4, 4);

    data.push({blocks::STONE_BLOCK, 1, 0});
    data.push({blocks::EARTH_BLOCK, 2, 0});
    data.push({blocks::GRASS_BLOCK, 0, 1});
    data.push({blocks::GRASS_BLOCK, 0, 2});
    data.push({blocks::EARTH_BLOCK, 3, 1});
    data.push({blocks::EARTH_BLOCK, 3, 2});
    data.push({blocks::STONE_BLOCK, 1, 3});
    data.push({blocks::STONE_BLOCK, 2, 3});

    return data;
  }
};

static inline std::unique_ptr<Structure>
circle_structure_factory(ubyte = 0) noexcept
{
  return std::make_unique<CircleStructure>();
}
}

static void _assert_extra_blocks(
  uint worldW, ushort worldH, int structureX, int structureY, ushort expectedVal
) noexcept
{
  PlainWorldGeneratorSettings settings;
  StructuresGenerator structuresGen;

  settings.w      = worldW;
  settings.layers = {{blocks::DEFAULT_BLOCK, worldH}};

  structuresGen.factory = _mock::circle_structure_factory;
  structuresGen.push({0, structureX, structureY});
  settings.structuresGenerator = &structuresGen;

  ASSERT_EQ(settings.extra_blocks(), expectedVal);
}

TEST(PlainWorldGenerator, ExtraBlockCountTest)
{
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
  StructuresGenerator structuresGen;

  settings.w = 20;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 10});

  structuresGen.factory = _mock::circle_structure_factory;
  structuresGen.push({0, -2, 9});
  structuresGen.push({0, 0, 11});
  settings.structuresGenerator = &structuresGen;

  settings.overridenBlocks = {
    // clang-format off
    {blocks::DEFAULT_BLOCK, -10, 12},
    {blocks::DEFAULT_BLOCK, 100, -110},
    {blocks::DEFAULT_BLOCK, 0, 13},
    {blocks::DEFAULT_BLOCK, 20, 9},
    {blocks::DEFAULT_BLOCK, 19, 9},
    {blocks::DEFAULT_BLOCK, 0, 0}
    // clang-format on
  };

  ASSERT_EQ(settings.extra_blocks(), 16);
}

TEST(PlainWorldGenerator, StructureGenerationInsideWorldTest)
{
  PlainWorldGeneratorSettings settings;
  StructuresGenerator structuresGen;

  settings.w = 4;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 4});

  structuresGen.factory = _mock::circle_structure_factory;
  structuresGen.push({0, 0, 0});
  settings.structuresGenerator = &structuresGen;

  const auto kWorld = PlainWorldGenerator(settings)();

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

  for (byte y = 0; y < 4; y++)
  {
    for (byte x = 0; x < 4; x++)
    {
      const BlockData &kBlock = *kWorld->at(x, y);

      ASSERT_EQ(kBlock.id, EXPECTED_BLOCKS[y * 4 + x]);
      ASSERT_EQ(kBlock.x, x);
      ASSERT_EQ(kBlock.y, y);
    }
  }
}

TEST(PlainWorldGenerator, StructureGenerationOutsideWorldTest)
{
  PlainWorldGeneratorSettings settings;
  StructuresGenerator structuresGen;

  settings.layers.push_back({blocks::DEFAULT_BLOCK, 4});
  settings.w = 4;

  structuresGen.factory = _mock::circle_structure_factory;
  structuresGen.push({0, 5, 5});
  settings.structuresGenerator = &structuresGen;

  const auto kWorld = PlainWorldGenerator(settings)();

  for (byte i = 0; i < 16; i++)
  {
    ASSERT_EQ(kWorld->at(i)->id, blocks::DEFAULT_BLOCK);
  }

  const auto kExpectedData = _mock::CircleStructure().data();

  for (byte i = 16; i < 32; i++)
  {
    if (!kExpectedData.at(i))
    {
      continue;
    }

    const BlockData &kBlock = *kWorld->at(i);

    ASSERT_EQ(kBlock.id, kExpectedData.at(i)->id);
    ASSERT_EQ(kBlock.x, 5 + kExpectedData.at(i)->x);
    ASSERT_EQ(kBlock.y, 5 + kExpectedData.at(i)->y);
  }
}

TEST(PlainWorldGenerator, StructureOverwriteTest)
{
  PlainWorldGeneratorSettings settings;
  StructuresGenerator structuresGen;

  settings.w = 4;
  settings.layers.push_back({blocks::DEFAULT_BLOCK, 4});

  structuresGen.factory = _mock::circle_structure_factory;
  structuresGen.push({0, 0, 0});
  structuresGen.push({0, 1, 1});
  settings.structuresGenerator = &structuresGen;

  const auto kWorld = PlainWorldGenerator(settings)();

  constexpr blocks::BlockId EXPECTED_BLOCKS[] = {
    // clang-format off
    blocks::DEFAULT_BLOCK,
    blocks::STONE_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::STONE_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::EARTH_BLOCK,
    blocks::DEFAULT_BLOCK,
    blocks::GRASS_BLOCK,
    blocks::STONE_BLOCK,
    blocks::DEFAULT_BLOCK
    // clang-format on
  };

  for (byte y = 0; y < 4; y++)
  {
    for (byte x = 0; x < 4; x++)
    {
      const BlockData &kBlock = *kWorld->at(x, y);

      ASSERT_EQ(kBlock.id, EXPECTED_BLOCKS[y * 4 + x]);
      ASSERT_EQ(kBlock.x, x);
      ASSERT_EQ(kBlock.y, y);
    }
  }
}
