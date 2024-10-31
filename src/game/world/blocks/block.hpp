#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "core/decoders/png.hpp"
#include "core/drawable.hpp"
#include "core/renderer.hpp"
#include "game/world/blocks/block_data.hpp"

#include <memory>

namespace game
{
class Block : public core::IDrawable
{
public:
  Block() noexcept;
  virtual ~Block() noexcept;

  static constexpr char SIZE = 24;

  void draw(const core::Renderer &renderer) const noexcept override;

  virtual const char *texture_path() const noexcept = 0;

  static std::unique_ptr<Block> from_data(const BlockData &data) noexcept;

  void set_pos(long long x, long long y) noexcept;
  void load_texture(core::PngDecoder &pngDecoder) noexcept;

private:
  long long _x, _y;

  unsigned _coordsVbo;
  unsigned _tex;
  unsigned _texCoordsVbo;
  unsigned _vao;

  void _load_default_tex() noexcept;
};
}

#endif
