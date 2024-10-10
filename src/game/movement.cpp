#include "game/movement.hpp"

namespace game {
Movement::Movement(float sensitivity) noexcept : _kSensitivity(sensitivity) {
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

  _view.translate(kXDiff, kYDiff);

  _prevX = x;
  _prevY = y;

  return _view;
}

void Movement::set_next_origin() noexcept {
  _prevPosesInitialized = false;
}
}
