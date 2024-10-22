#include "game/movement.hpp"

namespace game {
Movement::Movement(float sensitivity) noexcept
  : _kSensitivity(sensitivity), _offset() {
}

void Movement::operator()(long long x, long long y) noexcept {
  if (!_prevPosesInitialized) {
    _prevX                = x;
    _prevY                = y;
    _prevPosesInitialized = true;
  }

  _offset.x += (x - _prevX) * -_kSensitivity;
  _offset.y += (y - _prevY) * _kSensitivity;

  _prevX = x;
  _prevY = y;
}

void Movement::set_next_origin() noexcept {
  _prevPosesInitialized = false;
}

const MovementOffset &Movement::get() const noexcept {
  return _offset;
}
}
