#include "opengl/math/matrix.hpp"

namespace gl::math
{
float ViewMatrix::get_offset_x() const noexcept { return _data[0][3]; }
float ViewMatrix::get_offset_y() const noexcept { return _data[1][3]; }

void ViewMatrix::set_offset(float x, float y) noexcept
{
  _data[0][3] = -x;
  _data[1][3] = -y;
}

float ViewMatrix::get_scale() const noexcept { return _data[0][0]; }

void ViewMatrix::set_scale(float v) noexcept
{
  _data[0][0] = v;
  _data[1][1] = v;
}
}
