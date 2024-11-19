#ifndef _EFE_WORLD_HPP_
#define _EFE_WORLD_HPP_

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
  World(const BlockDataWorldData &blocksData) noexcept;

  BlockWorldData data;

  void draw(gl::Drawer &drawer) const noexcept override;
  inline bool always_draw() const noexcept override { return true; }

  void load_textures(core::PngDecoder &pngDecoder) noexcept;

private:
  core::Logger _logger;
};
}

#endif
