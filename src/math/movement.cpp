#include "math/movement.hpp"

namespace math
{
void Movement::operator()(float x, float y) noexcept
{
  if (!_prevPosesInitialized)
  {
    _prevX                = x;
    _prevY                = y;
    _prevPosesInitialized = true;
  }

  _offset.x += (x - _prevX) * -_kSensitivity;
  _offset.y += (y - _prevY) * _kSensitivity;

  _prevX = x;
  _prevY = y;
}

void Movement::reset_origin() noexcept { _prevPosesInitialized = false; }
}
