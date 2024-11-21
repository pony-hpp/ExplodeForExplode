#include "game/world/blocks/blocks.hpp"

namespace game::blocks
{
#define ID(className, v) \
  BlockId className::id() const noexcept { return v; }

#define TEXTURE(className, v) \
  const char *className::texture_path() const noexcept { return v; }

// clang-format off
ID(DefaultBlock, DEFAULT_BLOCK)
ID(GrassBlock,   GRASS_BLOCK)
ID(EarthBlock,   EARTH_BLOCK)
ID(StoneBlock,   STONE_BLOCK)
ID(OakBlock,     OAK_BLOCK)
ID(OakLeaves,    OAK_LEAVES)
ID(Grass,        GRASS)
ID(Water,        WATER)
ID(WaterWave,    WATER_WAVE)

TEXTURE(DefaultBlock, "")
TEXTURE(GrassBlock,   "../assets/textures/blocks/grass_block.png")
TEXTURE(EarthBlock,   "../assets/textures/blocks/earth.png")
TEXTURE(StoneBlock,   "../assets/textures/blocks/stone.png")
TEXTURE(OakBlock,     "../assets/textures/blocks/oak.png")
TEXTURE(OakLeaves,    "../assets/textures/blocks/oak_leaves.png")
TEXTURE(Grass,        "../assets/textures/blocks/grass.png")
TEXTURE(Water,        "../assets/textures/blocks/water.png")
TEXTURE(WaterWave,    "../assets/textures/blocks/water_wave.png")
// clang-format on
}
