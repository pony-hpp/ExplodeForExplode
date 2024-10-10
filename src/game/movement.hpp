#ifndef _MOVEMENT_HPP_
#define _MOVEMENT_HPP_

#include "opengl/math/matrix.hpp"

namespace game {
class Movement final {
public:
  explicit Movement(float sensitivity) noexcept;

  const gl::math::ViewMatrix &operator()(long long x, long long y) noexcept;
  void set_next_origin() noexcept;

private:
  const float _kSensitivity;
  gl::math::ViewMatrix _view;
  long long _prevX, _prevY;
  bool _prevPosesInitialized = false;
};
}

#endif
