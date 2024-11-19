#include "math/zooming.hpp"

namespace math
{
void Zooming::operator()(bool scale, float x, float y) noexcept
{
  _zoom.scale += _kSensitivity * (scale == true ? 1 : -1);

  if (_zoom.scale < _kMaxOut)
  {
    _zoom.scale = _kMaxOut;
    _translate_to_point(x, y);
    _clamped = true;
  }
  else if (_zoom.scale > _kMaxIn)
  {
    _zoom.scale = _kMaxIn;
    _translate_to_point(x, y);
    _clamped = true;
  }
  else
  {
    _clamped = false;
    _translate_to_point(x, y);
  }
}

void Zooming::_translate_to_point(float x, float y) noexcept
{
  if (!_clamped)
  {
    _zoom.offsetX = -x * (1 - _zoom.scale);
    _zoom.offsetY = -y * (1 - _zoom.scale);
  }
}
}
