#include "opengl/math/math.hpp"

float gl::math::normalize(float v, float max) noexcept {
  return v / max;
}
