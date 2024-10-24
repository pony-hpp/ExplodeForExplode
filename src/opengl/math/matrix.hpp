#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <initializer_list>

namespace gl::math
{
class Matrix
{
public:
  class Row final
  {
  public:
    float operator[](char x) const noexcept;
    float &operator[](char x) noexcept;

  private:
    float _data[4];
  };

  Matrix() noexcept;
  Matrix(std::initializer_list<float> v) noexcept;

  operator const float *() const noexcept;
  const Row &operator[](char y) const noexcept;
  Row &operator[](char y) noexcept;

protected:
  Row _data[4];
};

class ProjectionMatrix : public Matrix
{
public:
  ProjectionMatrix(unsigned short maxX, unsigned short maxY) noexcept;
};

class ViewMatrix final : public Matrix
{
public:
  float get_offset_x() const noexcept;
  float get_offset_y() const noexcept;
  void set_offset(float x, float y) noexcept;

  float get_scale() const noexcept;
  void set_scale(float v) noexcept;
};
}

#endif
