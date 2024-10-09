#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <initializer_list>

namespace gl::math {
class Matrix {
private:
  class _MatrixRow final {
  public:
    float &operator[](unsigned char x) noexcept;
    float operator[](unsigned char x) const noexcept;

  private:
    float _data[4];
  };

public:
  Matrix() noexcept;
  Matrix(std::initializer_list<float> v) noexcept;

  _MatrixRow &operator[](unsigned char y) noexcept;
  const _MatrixRow &operator[](unsigned char y) const noexcept;
  operator const float *() const noexcept;

protected:
  _MatrixRow _data[4];
};

class ProjectionMatrix : public Matrix {
public:
  ProjectionMatrix(unsigned short maxX, unsigned short maxY) noexcept;
};

class ViewMatrix final : public Matrix {
public:
  void translate(float x, float y) noexcept;
  float get_offset_x() const noexcept;
  float get_offset_y() const noexcept;
};
}

#endif
