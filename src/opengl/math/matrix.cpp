#include "opengl/math/matrix.hpp"

namespace gl::math {
static const Matrix _DEFAULT_MATRIX = {
  // clang-format off
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
  // clang-format on
};

Matrix::Matrix() noexcept : Matrix(_DEFAULT_MATRIX) {
}

Matrix::Matrix(std::initializer_list<float> v) noexcept {
  for (unsigned char i = 0; i < 4; i++) {
    for (unsigned char j = 0; j < 4; j++) {
      _data[i][j] = *(v.begin() + i * 4 + j);
    }
  }
}

Matrix::_MatrixRow &Matrix::operator[](unsigned char y) noexcept {
  return _data[y];
}

const Matrix::_MatrixRow &Matrix::operator[](unsigned char y) const noexcept {
  return _data[y];
}

Matrix::operator const float *() const noexcept {
  // We can safely cast _data (aka _MatrixRow[4]) because _MatrixHow takes 16
  // bytes, which is 4 floats.
  return (const float *)_data;
}

float &Matrix::_MatrixRow::operator[](unsigned char x) noexcept {
  return _data[x];
}

float Matrix::_MatrixRow::operator[](unsigned char x) const noexcept {
  return _data[x];
}

ProjectionMatrix::ProjectionMatrix(
  unsigned short maxX, unsigned short maxY
) noexcept
  : Matrix({
      // clang-format off
      2 / (float)maxX, 0.0f,            0.0f, -1.0f,
      0.0f,            2 / (float)maxY, 0.0f, -1.0f,
      0.0f,            0.0f,            1.0f,  0.0f,
      0.0f,            0.0f,            0.0f,  1.0f
      // clang-format on
    }) {
}

void ViewMatrix::set_offset(float x, float y) noexcept {
  _data[0][3] = x;
  _data[1][3] = y;
}

float ViewMatrix::get_offset_x() const noexcept {
  return _data[0][3];
}

float ViewMatrix::get_offset_y() const noexcept {
  return _data[1][3];
}
}
