#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "blocks/block.hpp"
#include "core/decoders/png.hpp"
#include "core/drawable.hpp"
#include "core/logger.hpp"
#include "core/renderer.hpp"
#include "game/world_generators/world_data.hpp"

#include <memory>

namespace game {
class World final : public core::IDrawable {
public:
  World(const WorldData &data) noexcept;
  ~World() noexcept;

  void draw(const core::Renderer &renderer) const noexcept override;

  void load_textures(core::PngDecoder &pngDecoder) noexcept;

private:
  const unsigned _kW;
  const unsigned short _kH;
  std::unique_ptr<Block> **_data;
  core::Logger _logger;
};
}

#endif
