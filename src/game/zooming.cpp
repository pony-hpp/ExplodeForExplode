#include "game/zooming.hpp"

namespace game
{
Zooming::Zooming(float sensitivity, float min, float max) noexcept
  : _kSensitivity(sensitivity), _kMin(min), _kMax(max),
    _zoom({1.0f, 0.0f, 0.0f})
{
}

void Zooming::operator()(bool scale, float x, float y) noexcept
{
  _zoom.scale += _kSensitivity * (scale == true ? 1 : -1);

  if (_zoom.scale < _kMin)
  {
    _zoom.scale = _kMin;
    _translate_to_point(x, y);
    _clamped = true;
  }
  else if (_zoom.scale > _kMax)
  {
    _zoom.scale = _kMax;
    _translate_to_point(x, y);
    _clamped = true;
  }
  else
  {
    _clamped = false;
    _translate_to_point(x, y);
  }
}

const Zoom &Zooming::get() const noexcept { return _zoom; }

void Zooming::_translate_to_point(float x, float y) noexcept
{
  if (!_clamped)
  {
    _zoom.offsetX = -x * (1 - _zoom.scale);
    _zoom.offsetY = -y * (1 - _zoom.scale);
  }
}
}
