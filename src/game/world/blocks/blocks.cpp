#include "game/world/blocks/blocks.hpp"

namespace game::blocks
{
#define ID(className, v) \
  BlockId className::id() const noexcept { return v; }

#define TEXTURE(className, v) \
  const char *className::texture_path() const noexcept { return v; }

#define DURABILITY(className, v) \
  float className::durability() const noexcept { return v; }

static constexpr float NONEXPLOSIVE = -1.0f;

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

DURABILITY(DefaultBlock, 0.0f)
DURABILITY(GrassBlock,   0.6f)
DURABILITY(EarthBlock,   0.45f)
DURABILITY(StoneBlock,   0.85f)
DURABILITY(OakBlock,     0.7f)
DURABILITY(OakLeaves,    0.03f)
DURABILITY(Grass,        0.0f)
DURABILITY(Water,        NONEXPLOSIVE)
DURABILITY(WaterWave,    NONEXPLOSIVE)
// clang-format on
}
