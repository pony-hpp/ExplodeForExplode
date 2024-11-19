#include "game/world/blocks/block.hpp"
#include "game/world/blocks/blocks.hpp"

#include <cstring>

namespace game
{
gl::ShaderProgram *Block::_defaultShaderProgram;
std::unordered_map<blocks::BlockId, gl::ShaderProgram *> Block::_shaderPrograms;

void Block::draw() const noexcept
{
  const auto kShaderProgram = _shaderPrograms.find(id());
  if (kShaderProgram != _shaderPrograms.cend())
  {
    kShaderProgram->second->use();
  }
  else
  {
    _defaultShaderProgram->use();
  }

  _mesh.draw(gl::DRAW_MODE_TRIANGLE_STRIP, 4);
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

void Block::set_pos(int x, int y) noexcept
{
  _x = x;
  _y = y;

  const float kXCoord = x * SIZE, kYCoord = y * SIZE;

  _mesh.vertices = {
    {kXCoord, kYCoord},
    {kXCoord + SIZE, kYCoord},
    {kXCoord, kYCoord + SIZE},
    {kXCoord + SIZE, kYCoord + SIZE}
  };

  _mesh.update_vertices(gl::UPD_INTENSITY_NONE);
}

void Block::load_texture(core::PngDecoder &pngDecoder) noexcept
{
  if (strcmp(texture_path(), "") != 0)
  {
    try
    {
      _mesh.load_texture(&pngDecoder(texture_path()));
    }
    catch (...)
    {
      _mesh.load_texture();
    }
  }
  else
  {
    _mesh.load_texture();
  }
}
}
