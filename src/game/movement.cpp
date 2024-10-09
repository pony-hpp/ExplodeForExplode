#include "game/movement.hpp"
#include "opengl/math/math.hpp"

namespace game {
Movement::Movement(
  const core::ISizeable<unsigned short> &viewport, float sensitivity
) noexcept
  : _kViewport(viewport), _kSensitivity(sensitivity) {
}

const gl::math::ViewMatrix &
Movement::operator()(long long x, long long y) noexcept {
  if (!_prevPosesInitialized) {
    _prevX                = x;
    _prevY                = y;
    _prevPosesInitialized = true;
  }

  const float kXDiff = (x - _prevX) * _kSensitivity,
              kYDiff = -(y - _prevY) * _kSensitivity;

  _view.translate(
    gl::math::normalize(kXDiff, _kViewport.w()),
    gl::math::normalize(kYDiff, _kViewport.h())
  );

  _prevX = x;
  _prevY = y;

  return _view;
}

void Movement::set_next_origin() noexcept {
  _prevPosesInitialized = false;
}
}