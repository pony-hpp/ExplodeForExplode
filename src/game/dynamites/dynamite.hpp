#ifndef _EFE_DYNAMITE_HPP_
#define _EFE_DYNAMITE_HPP_

#include "core/decoders/png.hpp"
#include "core/types.hpp"
#include "game/world/world.hpp"
#include "opengl/drawable.hpp"
#include "opengl/mesh.hpp"
#include "opengl/shading/shader_program.hpp"

namespace game
{
class Dynamite final : public gl::ISingleDrawable
{
public:
  inline explicit Dynamite(World &world) noexcept : _world(world) {};

  static constexpr byte EXPLOSION_RADIUS  = 2;
  static constexpr byte FALLBACK_TEX_SIZE = 24;

  void draw() const noexcept override;
  inline int x() const noexcept override { return _x - w() / 2.0f; }
  inline int y() const noexcept override { return _y - h() / 2.0f; }

  inline uint w() const noexcept override
  {
    return _texLoaded ? _texW : FALLBACK_TEX_SIZE;
  }

  inline uint h() const noexcept override
  {
    return _texLoaded ? _texH : FALLBACK_TEX_SIZE;
  }

  inline static void shader_program(gl::ShaderProgram &shaderProgram) noexcept
  {
    _shaderProgram = &shaderProgram;
  }

  void load_texture(core::PngDecoder &pngDecoder) noexcept;
  void set_pos(int x, int y) noexcept;
  void explode() noexcept;

private:
  static gl::ShaderProgram *_shaderProgram;

  World &_world;
  int _x, _y;
  ushort _texW, _texH;
  bool _texLoaded = false;
  gl::Mesh _mesh;

  void _explode_children(const BlockData *data) noexcept;
};
}

#endif
