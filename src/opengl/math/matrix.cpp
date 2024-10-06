#include "opengl/math/matrix.hpp"

gl::math::Matrix
gl::math::projection_matrix(unsigned short maxX, unsigned short maxY) noexcept {
  return {
    // clang-format off
    2 / (float)maxX, 0.0f,            0.0f, -1.0f,
    0.0f,            2 / (float)maxY, 0.0f, -1.0f,
    0.0f,            0.0f,            1.0f,  0.0f,
    0.0f,            0.0f,            0.0f,  1.0f
    // clang-format on
  };
}
