#ifndef EFE_BLOCK_HPP
#define EFE_BLOCK_HPP

#include "core/decoders/png.hpp"
#include "game/world/blocks/block_data.hpp"
#include "opengl/drawable.hpp"
#include "opengl/mesh.hpp"
#include "opengl/shading/shader_program.hpp"

#include <memory>
#include <unordered_map>

namespace game
{
class Block : public gl::ISingleDrawable
{
public:
  virtual ~Block() = default;

  static constexpr byte SIZE = 24;

  void draw() const noexcept override;
  inline int x() const noexcept override { return _x * SIZE; }
  inline int y() const noexcept override { return _y * SIZE; }
  inline uint w() const noexcept override { return SIZE; }
  inline uint h() const noexcept override { return SIZE; }

  virtual blocks::BlockId id() const noexcept       = 0;
  virtual const char *texture_path() const noexcept = 0;

  // Negative durability values make the block non-explosive.
  virtual float durability() const noexcept = 0;

  static std::unique_ptr<Block> from_data(const BlockData &data) noexcept;

  inline static void default_shader_program(gl::ShaderProgram &shaderProgram
  ) noexcept
  {
    _defaultShaderProgram = &shaderProgram;
  }

  inline static void match_shader_program(
    blocks::BlockId id, gl::ShaderProgram &shaderProgram
  ) noexcept
  {
    _shaderPrograms.insert_or_assign(id, &shaderProgram);
  }

  inline int pos_x() const noexcept { return _x; }
  inline int pos_y() const noexcept { return _y; }

  void set_pos(int x, int y) noexcept;
  void load_texture(core::PngDecoder &pngDecoder) noexcept;
  bool explode(float power) noexcept;

private:
  static gl::ShaderProgram *_defaultShaderProgram;
  static std::unordered_map<blocks::BlockId, gl::ShaderProgram *>
    _shaderPrograms;

  int _x, _y;
  float _curDurability;
  bool _curDurabilityInitialized = false;
  gl::Mesh _mesh;
};
}

#endif
