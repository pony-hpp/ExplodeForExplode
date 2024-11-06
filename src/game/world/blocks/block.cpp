#include "core/renderer.hpp"
#include "game/world/blocks/block.hpp"
#include "game/world/blocks/blocks.hpp"
#include "opengl/opengl.hpp"
#include "opengl/texture.hpp"

#include <cstring>

namespace game
{
gl::ShaderProgram *Block::_defaultShaderProgram;
std::unordered_map<blocks::BlockId, gl::ShaderProgram *> Block::_shaderPrograms;

Block::Block() noexcept
{
  glGenBuffers(1, &_coordsVbo);
  glGenTextures(1, &_tex);
  glGenBuffers(1, &_texCoordsVbo);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Block::~Block() noexcept
{
  glDeleteBuffers(1, &_coordsVbo);
  glDeleteTextures(1, &_tex);
  glDeleteBuffers(1, &_texCoordsVbo);
  glDeleteVertexArrays(1, &_vao);
}

void Block::draw(const core::Renderer &renderer) const noexcept
{
  const float kViewBlockXCoord = _x * renderer.view.get_scale() * SIZE +
                                 renderer.view.get_offset_x(),
              kViewBlockYCoord = _y * renderer.view.get_scale() * SIZE +
                                 renderer.view.get_offset_y();

  if (kViewBlockXCoord >= -SIZE * renderer.view.get_scale() &&
      kViewBlockXCoord <= renderer.viewport_w() &&
      kViewBlockYCoord >= -SIZE * renderer.view.get_scale() &&
      kViewBlockYCoord <= renderer.viewport_h())
  {
    const auto kShaderProgram = _shaderPrograms.find(id());
    if (kShaderProgram != _shaderPrograms.end())
    {
      kShaderProgram->second->use();
    }
    else
    {
      _defaultShaderProgram->use();
    }

    glBindVertexArray(_vao);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

std::unique_ptr<Block> Block::from_data(const BlockData &data) noexcept
{
  std::unique_ptr<Block> res;
  switch (data.id)
  {
  case blocks::DEFAULT_BLOCK:
    res = std::make_unique<blocks::DefaultBlock>();
    break;
  case blocks::GRASS_BLOCK: res = std::make_unique<blocks::GrassBlock>(); break;
  case blocks::EARTH_BLOCK: res = std::make_unique<blocks::EarthBlock>(); break;
  case blocks::STONE_BLOCK: res = std::make_unique<blocks::StoneBlock>(); break;
  case blocks::OAK_BLOCK: res = std::make_unique<blocks::OakBlock>(); break;
  case blocks::OAK_LEAVES: res = std::make_unique<blocks::OakLeaves>(); break;
  case blocks::GRASS: res = std::make_unique<blocks::Grass>(); break;
  case blocks::WATER: res = std::make_unique<blocks::Water>(); break;
  case blocks::WATER_WAVE: res = std::make_unique<blocks::WaterWave>(); break;
  default: return nullptr;
  }

  res->set_pos(data.x, data.y);
  return res;
}

void Block::default_shader_program(gl::ShaderProgram &shaderProgram) noexcept
{
  _defaultShaderProgram = &shaderProgram;
}

void Block::match_shader_program(
  blocks::BlockId id, gl::ShaderProgram &shaderProgram
) noexcept
{
  _shaderPrograms.insert({id, &shaderProgram});
}

int Block::x() const noexcept { return _x; }
int Block::y() const noexcept { return _y; }

void Block::set_pos(int x, int y) noexcept
{
  _x = x;
  _y = y;

  const int kXCoord = x * SIZE, kYCoord = y * SIZE;
  const int kCoords[] = {
    // clang-format off
    kXCoord,        kYCoord,
    kXCoord + SIZE, kYCoord,
    kXCoord,        kYCoord + SIZE,
    kXCoord + SIZE, kYCoord + SIZE
    // clang-format on
  };

  glBindBuffer(GL_ARRAY_BUFFER, _coordsVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kCoords), kCoords, GL_STATIC_DRAW);
  glBindVertexArray(_vao);
  glVertexAttribPointer(0, 2, GL_INT, false, 0, nullptr);
}

void Block::load_texture(core::PngDecoder &pngDecoder) noexcept
{
  glBindBuffer(GL_ARRAY_BUFFER, _texCoordsVbo);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(gl::TEX_COORDS), gl::TEX_COORDS, GL_STATIC_DRAW
  );
  glBindTexture(GL_TEXTURE_2D, _tex);
  glBindVertexArray(_vao);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

  if (strcmp(texture_path(), "") != 0)
  {
    try
    {
      const core::Png &kTex = pngDecoder(texture_path());
      glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, kTex.w, kTex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        kTex.data.get()
      );
    }
    catch (...)
    {
      _load_default_tex();
    }
  }
  else
  {
    _load_default_tex();
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Block::_load_default_tex() noexcept
{
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    gl::FALLBACK_TEX
  );
}
}
