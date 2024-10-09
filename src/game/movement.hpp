#ifndef _MOVEMENT_HPP_
#define _MOVEMENT_HPP_

#include "core/sizeable.hpp"
#include "opengl/math/matrix.hpp"

namespace game {
template <typename ViewportSizeT>
class Movement final {
public:
  explicit Movement(
    const core::ISizeable<ViewportSizeT> &viewport, float sensitivity
  ) noexcept;

  const gl::math::ViewMatrix &operator()(long long x, long long y) noexcept;
  void set_next_origin() noexcept;

private:
  const core::ISizeable<ViewportSizeT> &_kViewport;
  const float _kSensitivity;
  gl::math::ViewMatrix _view;
  long long _prevX, _prevY;
  bool _prevPosesInitialized = false;
};
}

#ifndef _MOVEMENT_TPP_
#include "game/movement.tpp" // IWYU pragma: keep
#endif

#endif
