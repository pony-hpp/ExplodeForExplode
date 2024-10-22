#include "core/decoders/png.hpp"
#include "game/world/blocks/block.hpp"
#include "game/world/blocks/blocks.hpp"
#include "opengl/opengl.hpp"

#include <cstring>

static constexpr float _TEX_COORDS[] = {
  // clang-format off
  0.0f, 1.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f,
  // clang-format on
};

static constexpr unsigned char _FALLBACK_TEX[] = {
  // clang-format off
  0,   0, 0,   255,  255, 0,  255, 255,
  255, 0, 255, 255,  0,   0,  0,   255
  // clang-format on
};

namespace game {
Block::Block() noexcept {
  glGenBuffers(1, &_coordsVbo);
  glGenTextures(1, &_tex);
  glGenBuffers(1, &_texCoordsVbo);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Block::~Block() noexcept {
  glDeleteBuffers(1, &_coordsVbo);
  glDeleteTextures(1, &_tex);
  glDeleteBuffers(1, &_texCoordsVbo);
  glDeleteVertexArrays(1, &_vao);
}

void Block::draw(const core::Renderer &renderer) const noexcept {
  const float kViewXBlockCoord = _x * renderer.view.get_scale() * SIZE +
                                 renderer.view.get_offset_x(),
              kViewYBlockCoord = _y * renderer.view.get_scale() * SIZE +
                                 renderer.view.get_offset_y();

  if (kViewXBlockCoord >= -SIZE * renderer.view.get_scale() &&
      kViewXBlockCoord <= renderer.viewport_w() &&
      kViewYBlockCoord >= -SIZE * renderer.view.get_scale() &&
      kViewYBlockCoord <= renderer.viewport_h()) {
    glBindVertexArray(_vao);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

std::unique_ptr<Block> Block::from_data(const BlockData &data) noexcept {
  std::unique_ptr<Block> res;
  switch (data.id) {
  case blocks::DEFAULT_BLOCK:
    res = std::make_unique<blocks::DefaultBlock>();
    break;
  case blocks::GRASS_BLOCK: res = std::make_unique<blocks::GrassBlock>(); break;
  case blocks::EARTH_BLOCK: res = std::make_unique<blocks::EarthBlock>(); break;
  case blocks::STONE_BLOCK: res = std::make_unique<blocks::StoneBlock>(); break;
  default: return nullptr;
  }

  res->set_pos(data.x, data.y);
  return res;
}

void Block::set_pos(int x, int y) noexcept {
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

void Block::load_texture(core::PngDecoder &pngDecoder) noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, _texCoordsVbo);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(_TEX_COORDS), _TEX_COORDS, GL_STATIC_DRAW
  );
  glBindVertexArray(_vao);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);
  glBindTexture(GL_TEXTURE_2D, _tex);

  if (strcmp(texture_path(), "") != 0) {
    try {
      const core::Png &kTex = pngDecoder(texture_path());
      glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, kTex.w, kTex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        kTex.data.get()
      );
    } catch (...) {
      _load_default_tex();
    }
  } else {
    _load_default_tex();
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Block::_load_default_tex() noexcept {
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, _FALLBACK_TEX
  );
}
}
