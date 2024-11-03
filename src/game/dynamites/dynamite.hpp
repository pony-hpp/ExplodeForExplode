#ifndef _DYNAMITE_HPP_
#define _DYNAMITE_HPP_

#include "core/decoders/png.hpp"
#include "core/drawable.hpp"
#include "game/world/world.hpp"

namespace game
{
class Dynamite final : public core::IDrawable
{
public:
  explicit Dynamite(World &world) noexcept;
  ~Dynamite() noexcept;

  static constexpr char RADIUS            = 2;
  static constexpr char FALLBACK_TEX_SIZE = 24;

  virtual void draw(const core::Renderer &renderer) const noexcept;

  void load_texture(core::PngDecoder &pngDecoder) noexcept;
  void set_pos(int x, int y) noexcept;
  void explode() noexcept;

private:
  World &_world;
  int _x, _y;
  unsigned short _texW, _texH;
  bool _texLoaded = false;

  unsigned _coordsVbo;
  unsigned _tex;
  unsigned _texCoordsVbo;
  unsigned _vao;
};
}

#endif
