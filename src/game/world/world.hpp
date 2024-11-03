#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "core/decoders/png.hpp"
#include "core/drawable.hpp"
#include "core/logger.hpp"
#include "game/world/blocks/block.hpp"
#include "game/world/world_data.hpp"

#include <memory>

namespace game
{
class World final : public core::IDrawable
{
public:
  World(const WorldData &worldData) noexcept;

  void draw(const core::Renderer &renderer) const noexcept override;

  std::unique_ptr<Block> *at(int x, int y) noexcept;
  void load_textures(core::PngDecoder &pngDecoder) noexcept;

private:
  const unsigned _w;
  const unsigned short _h;
  const unsigned long long _kBlockCount;
  std::unique_ptr<std::unique_ptr<Block>[]> _data;
  core::Logger _logger;
};
}

#endif
