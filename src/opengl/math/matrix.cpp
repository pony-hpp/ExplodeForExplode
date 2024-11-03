#include "opengl/math/matrix.hpp"

namespace gl::math
{
static const Matrix _DEFAULT_MATRIX = {
  // clang-format off
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
  // clang-format on
};

Matrix::Matrix() noexcept : Matrix(_DEFAULT_MATRIX) {}

Matrix::Matrix(std::initializer_list<float> v) noexcept
{
  for (char y = 0; y < 4; y++)
  {
    for (char x = 0; x < 4; x++)
    {
      _data[y][x] = *(v.begin() + y * 4 + x);
    }
  }
}

Matrix::operator const float *() const noexcept
{
  // We can safely cast _data (aka _MatrixRow[4]) to the float pointer because
  // _MatrixRow takes 16 bytes, which is 4 floats.
  return (float *)_data;
}

const Matrix::Row &Matrix::operator[](char y) const noexcept
{
  return _data[y];
}
Matrix::Row &Matrix::operator[](char y) noexcept { return _data[y]; }

float Matrix::Row::operator[](char x) const noexcept { return _data[x]; }
float &Matrix::Row::operator[](char x) noexcept { return _data[x]; }
}
