#include "game/blocks/blocks.hpp"

namespace game::blocks {
#define TEXTURE(className, v) \
  const char *className::texture() const noexcept { \
    return v; \
  }

// clang-format off
TEXTURE(DefaultBlock, "")
TEXTURE(GrassBlock,   "../assets/textures/blocks/grass_block.png")
TEXTURE(EarthBlock,   "../assets/textures/blocks/earth_block.png")
// clang-format on
}
