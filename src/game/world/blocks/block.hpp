#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "core/decoders/png.hpp"
#include "core/drawable.hpp"
#include "game/world/blocks/block_data.hpp"
#include "opengl/shading/shader_program.hpp"

#include <memory>
#include <unordered_map>

namespace game
{
class Block : public core::IDrawable
{
public:
  Block() noexcept;
  virtual ~Block() noexcept;

  static constexpr char SIZE = 24;

  void draw(const core::Renderer &renderer) const noexcept override;

  virtual blocks::BlockId id() const noexcept       = 0;
  virtual const char *texture_path() const noexcept = 0;

  static std::unique_ptr<Block> from_data(const BlockData &data) noexcept;
  static void default_shader_program(gl::ShaderProgram &shaderProgram) noexcept;
  static void match_shader_program(
    blocks::BlockId id, gl::ShaderProgram &shaderProgram
  ) noexcept;

  int x() const noexcept;
  int y() const noexcept;

  void set_pos(int x, int y) noexcept;
  void load_texture(core::PngDecoder &pngDecoder) noexcept;

private:
  static gl::ShaderProgram *_defaultShaderProgram;
  static std::unordered_map<blocks::BlockId, gl::ShaderProgram *>
    _shaderPrograms;

  int _x, _y;

  unsigned _coordsVbo;
  unsigned _tex;
  unsigned _texCoordsVbo;
  unsigned _vao;

  void _load_default_tex() noexcept;
};
}

#endif
