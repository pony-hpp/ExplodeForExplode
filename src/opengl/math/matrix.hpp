#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <array>

namespace gl::math {
using Matrix = std::array<float, 16>;

const Matrix DEFAULT_MATRIX = {
  // clang-format off
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
  // clang-format on
};

Matrix projection_matrix(unsigned short maxX, unsigned short maxY) noexcept;

void translate(Matrix &mat, float x, float y) noexcept;
}

#endif
