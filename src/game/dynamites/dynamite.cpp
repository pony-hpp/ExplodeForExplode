#include "game/dynamites/dynamite.hpp"

#include <cmath>

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
  _posX = x;
  _posY = y;

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
  for (byte y = -EXPLOSION_RADIUS; y < EXPLOSION_RADIUS; y++)
  {
    for (byte x = -EXPLOSION_RADIUS; x < EXPLOSION_RADIUS; x++)
    {
      // Don't touch the blocks outside the circle with the radius
      // `EXPLOSION_RADIUS`.
      if (pow(x, 2) + pow(y, 2) >= pow(EXPLOSION_RADIUS, 2))
      {
        continue;
      }

      const float kRadiusDist = sqrt(pow(x, 2) + pow(y, 2)) + 1.0f;
      const float kPower      = EXPLOSION_POWER / kRadiusDist;

      const int kXPos = _posX / Block::SIZE + x,
                kYPos = _posY / Block::SIZE + y;

      auto block = _world.blocks.at(kXPos, kYPos);
      if (block && *block && block->get()->explode(kPower))
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
  if (!data)
  {
    return;
  }

  for (const auto &childPos : data->childPoses)
  {
    auto *childBlock = _world.blocks.at(childPos.first, childPos.second);
    if (childBlock)
    {
      if (*childBlock)
      {
        childBlock->reset();
      }

      _explode_children(_world.blocksData.at(childPos.first, childPos.second));
    }
  }
}
}
