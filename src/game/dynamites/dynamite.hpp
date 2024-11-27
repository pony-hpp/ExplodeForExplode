#ifndef EFE_DYNAMITE_HPP
#define EFE_DYNAMITE_HPP

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

  static constexpr byte EXPLOSION_RADIUS  = 5;
  static constexpr float EXPLOSION_POWER  = 0.3f;
  static constexpr byte FALLBACK_TEX_SIZE = 24;

  void draw() const noexcept override;
  inline int x() const noexcept override { return _posX - w() / 2.0f; }
  inline int y() const noexcept override { return _posY - h() / 2.0f; }

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
  int _posX, _posY;
  ushort _texW, _texH;
  bool _texLoaded = false;
  gl::Mesh _mesh;

  void _explode_children(const BlockData *data) noexcept;
};
}

#endif
