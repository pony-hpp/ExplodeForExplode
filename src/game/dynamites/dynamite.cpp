#include "game/dynamites/dynamite.hpp"

namespace game
{
gl::ShaderProgram *Dynamite::_shaderProgram;

void Dynamite::draw() const noexcept
{
  _shaderProgram->use();
  _mesh.draw(gl::DRAW_MODE_TRIANGLE_STRIP, 4);
}

void Dynamite::load_texture(core::PngDecoder &pngDecoder) noexcept
{
  try
  {
    _mesh.load_texture(&pngDecoder("../assets/textures/dynamite.png"));
  }
  catch (...)
  {
    _mesh.load_texture();
  }
}

void Dynamite::set_pos(int x, int y) noexcept
{
  _x = x;
  _y = y;

  _mesh.vertices = {
    {x - w() / 2.0f, y - h() / 2.0f},
    {x + w() / 2.0f, y - h() / 2.0f},
    {x - w() / 2.0f, y + h() / 2.0f},
    {x + w() / 2.0f, y + h() / 2.0f}
  };

  _mesh.update_vertices(gl::UPD_INTENSITY_NONE);
}

void Dynamite::explode() noexcept
{
  for (byte y = -EXPLOSION_RADIUS - 1; y < EXPLOSION_RADIUS; y++)
  {
    for (byte x = -EXPLOSION_RADIUS - 1; x < EXPLOSION_RADIUS; x++)
    {
      if ((x == -EXPLOSION_RADIUS - 1 && y == -EXPLOSION_RADIUS - 1) ||
          (x == EXPLOSION_RADIUS - 1 && y == -EXPLOSION_RADIUS - 1) ||
          (x == -EXPLOSION_RADIUS - 1 && y == EXPLOSION_RADIUS - 1) ||
          (x == EXPLOSION_RADIUS - 1 && y == EXPLOSION_RADIUS - 1
          )) // Don't touch the corners
      {
        continue;
      }

      const int kXPos = _x / Block::SIZE + x, kYPos = _y / Block::SIZE + y;
      auto block = _world.blocks.at(kXPos, kYPos);
      if (block)
      {
        // It's safe to reset a `std::unique_ptr` that points to `nullptr`
        block->reset();

        _explode_children(_world.blocksData.at(kXPos, kYPos));
      }
    }
  }
}

void Dynamite::_explode_children(const BlockData *data) noexcept
{
  if (data && !data->childPoses.empty())
  {
    for (const auto &childPos : data->childPoses)
    {
      auto *childBlock = _world.blocks.at(childPos.first, childPos.second);
      if (childBlock)
      {
        childBlock->reset();
      }

      _explode_children(_world.blocksData.at(childPos.first, childPos.second));
    }
  }
}
}
