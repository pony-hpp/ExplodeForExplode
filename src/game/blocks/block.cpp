#include "core/decoders/png.hpp"
#include "game/blocks/block.hpp"
#include "game/blocks/blocks.hpp"
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
      kViewXBlockCoord <= renderer.viewportW() &&
      kViewYBlockCoord >= -SIZE * renderer.view.get_scale() &&
      kViewYBlockCoord <= renderer.viewportH()) {
    glBindTexture(GL_TEXTURE_2D, _tex);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

std::unique_ptr<Block> Block::from_id(blocks::BlockId id) noexcept {
  switch (id) {
  case blocks::DEFAULT_BLOCK:
    return std::make_unique<blocks::DefaultBlock>();
    break;
  case blocks::GRASS_BLOCK:
    return std::make_unique<blocks::GrassBlock>();
    break;
  case blocks::EARTH_BLOCK:
    return std::make_unique<blocks::EarthBlock>();
    break;
  case blocks::STONE_BLOCK:
    return std::make_unique<blocks::StoneBlock>();
    break;
  default: return nullptr;
  }
}

std::unique_ptr<Block> Block::from_data(const BlockData &data) noexcept {
  std::unique_ptr<Block> res = from_id(data.id);
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
  if (strcmp(texture(), "") != 0) {
    core::Png &tex = pngDecoder(texture());
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      tex.data.get()
    );
  } else {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      _FALLBACK_TEX
    );
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
}
