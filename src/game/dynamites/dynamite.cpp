#include "game/dynamites/dynamite.hpp"
#include "game/world/blocks/block.hpp"
#include "opengl/opengl.hpp"
#include "opengl/texture.hpp"

namespace game
{
Dynamite::Dynamite(World &world) noexcept : _world(world)
{
  // Yes. There's a little bit of copy-paste. But i'll add Mesh class soon.

  glGenBuffers(1, &_coordsVbo);
  glGenTextures(1, &_tex);
  glGenBuffers(1, &_texCoordsVbo);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
}

Dynamite::~Dynamite() noexcept
{
  glDeleteBuffers(1, &_coordsVbo);
  glDeleteTextures(1, &_tex);
  glDeleteBuffers(1, &_texCoordsVbo);
  glDeleteVertexArrays(1, &_vao);
}

void Dynamite::draw(const core::Renderer &) const noexcept
{
  glBindVertexArray(_vao);
  glBindTexture(GL_TEXTURE_2D, _tex);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Dynamite::load_texture(core::PngDecoder &pngDecoder) noexcept
{
  glBindBuffer(GL_ARRAY_BUFFER, _texCoordsVbo);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(gl::TEX_COORDS), gl::TEX_COORDS, GL_STATIC_DRAW
  );
  glBindTexture(GL_TEXTURE_2D, _tex);
  glBindVertexArray(_vao);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

  try
  {
    const core::Png &kTex = pngDecoder("../assets/textures/dynamite.png");
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, kTex.w, kTex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      kTex.data.get()
    );
    _texW      = kTex.w;
    _texH      = kTex.h;
    _texLoaded = true;
  }
  catch (...)
  {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      gl::FALLBACK_TEX
    );
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Dynamite::set_pos(int x, int y) noexcept
{
  _x = x;
  _y = y;

  const int kW        = _texLoaded ? _texW : FALLBACK_TEX_SIZE,
            kH        = _texLoaded ? _texH : FALLBACK_TEX_SIZE;
  const int kCoords[] = {
    // clang-format off
    x - kW / 2, y - kH / 2,
    x + kW / 2, y - kH / 2,
    x - kW / 2, y + kH / 2,
    x + kW / 2, y + kH / 2
    // clang-format on
  };

  glBindBuffer(GL_ARRAY_BUFFER, _coordsVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kCoords), kCoords, GL_STATIC_DRAW);
  glBindVertexArray(_vao);
  glVertexAttribPointer(0, 2, GL_INT, false, 0, nullptr);
}

void Dynamite::explode() noexcept
{
  for (char y = -RADIUS - 1; y < RADIUS; y++)
  {
    for (char x = -RADIUS - 1; x < RADIUS; x++)
    {
      if ((x == -RADIUS - 1 && y == -RADIUS - 1) ||
          (x == RADIUS - 1 && y == -RADIUS - 1) ||
          (x == -RADIUS - 1 && y == RADIUS - 1) ||
          (x == RADIUS - 1 && y == RADIUS - 1)) // Don't touch corners.
      {
        continue;
      }

      auto block = _world.at(_x / Block::SIZE + x, _y / Block::SIZE + y);
      if (block)
      {
        (*block).reset();
      }
    }
  }
}
}
