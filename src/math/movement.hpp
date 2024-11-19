#ifndef _EFE_MOVEMENT_HPP_
#define _EFE_MOVEMENT_HPP_

namespace math
{
struct MovementOffset
{
  float x, y;
};

class Movement final
{
public:
  inline explicit Movement(float sensitivity) noexcept
    : _kSensitivity(sensitivity), _offset()
  {}

  void operator()(float x, float y) noexcept;
  void reset_origin() noexcept;
  inline const MovementOffset &get() const noexcept { return _offset; }

private:
  const float _kSensitivity;
  MovementOffset _offset;

  float _prevX, _prevY;
  bool _prevPosesInitialized = false;
};
}

#endif
