#include "game/blocks/block.hpp"
#include "opengl/opengl.hpp"

namespace game {
Block::Block() noexcept {
  glGenBuffers(1, &_coordsVbo);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
}

Block::~Block() noexcept {
  glDeleteBuffers(1, &_coordsVbo);
  glDeleteVertexArrays(1, &_vao);
}

void Block::draw() const noexcept {
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Block::set_pos(int x, int y) noexcept {
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
}
