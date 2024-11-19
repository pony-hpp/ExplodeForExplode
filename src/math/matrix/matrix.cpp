#include "math/matrix/matrix.hpp"

namespace math
{
Matrix::Matrix() noexcept
  : Matrix({
      // clang-format off
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      // clang-format on
    })
{}

Matrix::Matrix(std::initializer_list<float> v) noexcept
{
  for (byte y = 0; y < 4; y++)
  {
    for (byte x = 0; x < 4; x++)
    {
      _data[y][x] = *(v.begin() + y * 4 + x);
    }
  }
}
}
