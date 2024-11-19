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

      auto block = _world.data.at(_x / Block::SIZE + x, _y / Block::SIZE + y);
      if (block)
      {
        // It's safe to reset a `std::unique_ptr` that points to `nullptr`
        block->reset();
      }
    }
  }
}
}
