#ifndef _MOVEMENT_TPP_
#define _MOVEMENT_TPP_

#include "game/movement.hpp"
#include "opengl/math/math.hpp"

namespace game {
template <typename ViewportSizeT>
Movement<ViewportSizeT>::Movement(
  const core::ISizeable<ViewportSizeT> &viewport, float sensitivity
) noexcept
  : _kViewport(viewport), _kSensitivity(sensitivity) {
}

template <typename T>
const gl::math::ViewMatrix &
Movement<T>::operator()(long long x, long long y) noexcept {
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

template <typename T>
void Movement<T>::set_next_origin() noexcept {
  _prevPosesInitialized = false;
}
}

#endif
