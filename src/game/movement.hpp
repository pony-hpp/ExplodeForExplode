#ifndef _MOVEMENT_HPP_
#define _MOVEMENT_HPP_

#include "core/sizeable.hpp"
#include "opengl/math/matrix.hpp"

namespace game {
class Movement final {
public:
  explicit Movement(
    const core::ISizeable<unsigned short> &viewport, float sensitivity
  ) noexcept;

  const gl::math::Matrix &operator()(long long x, long long y) noexcept;
  void set_next_origin() noexcept;

private:
  const core::ISizeable<unsigned short> &_kViewport;
  const float _kSensitivity;
  gl::math::Matrix _view;
  long long _prevX, _prevY;
  bool _prevPosesInitialized = false;
};
}

#endif
