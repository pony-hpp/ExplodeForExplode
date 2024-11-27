#ifndef EFE_MATRIX_HPP
#define EFE_MATRIX_HPP

#include "core/types.hpp"

#include <initializer_list>

namespace math
{
class Matrix
{
public:
  class Row final
  {
  public:
    inline float operator[](byte x) const noexcept { return _data[x]; }
    inline float &operator[](byte x) noexcept { return _data[x]; }

  private:
    float _data[4];
  };

  Matrix() noexcept;
  Matrix(std::initializer_list<float> v) noexcept;

  inline explicit operator float *() noexcept
  {
    // We can safely cast `_data` (aka `Row[]`) to the float pointer, because
    // `Row` stores its data consistently
    return (float *)_data;
  }

  inline const Row &operator[](byte y) const noexcept { return _data[y]; }
  inline Row &operator[](byte y) noexcept { return _data[y]; }

protected:
  Row _data[4];
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

class ProjectionMatrix final : public Matrix
{
public:
  explicit ProjectionMatrix(ushort maxX, ushort maxY) noexcept;
};
}

#endif
