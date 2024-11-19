#include "math/matrix/matrix.hpp"

namespace math
{
ProjectionMatrix::ProjectionMatrix(ushort maxX, ushort maxY) noexcept
  : Matrix({
      // clang-format off
      2 / (float)maxX, 0.0f,            0.0f, -1.0f,
      0.0f,            2 / (float)maxY, 0.0f, -1.0f,
      0.0f,            0.0f,            1.0f,  0.0f,
      0.0f,            0.0f,            0.0f,  1.0f
      // clang-format on
    })
{}
}
