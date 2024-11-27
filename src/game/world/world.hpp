#ifndef EFE_WORLD_HPP
#define EFE_WORLD_HPP

#include "core/decoders/png.hpp"
#include "core/logger.hpp"
#include "game/utils/data_2d.hpp"
#include "game/world/blocks/block.hpp"
#include "opengl/drawable.hpp"

#include <memory>

namespace game
{
// Maybe a little strange name :)
using BlockDataWorldData = utils::Data2D<BlockData>;

using BlockWorldData = utils::Data2D<std::unique_ptr<Block>>;

class World final : public gl::ICompositeDrawable
{
public:
  World(BlockDataWorldData &&blocksData) noexcept;

  BlockWorldData blocks;
  BlockDataWorldData blocksData;

  void draw(gl::Drawer &drawer) const noexcept override;
  inline bool always_draw() const noexcept override { return true; }

  void load_textures(core::PngDecoder &pngDecoder) noexcept;

private:
  core::Logger _logger;
};
}

#endif
