#ifndef EFE_MOVEMENT_HPP
#define EFE_MOVEMENT_HPP

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
