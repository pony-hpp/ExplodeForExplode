#ifndef _MOVEMENT_HPP_
#define _MOVEMENT_HPP_

namespace game
{
struct MovementOffset
{
  float x, y;
};

class Movement final
{
public:
  explicit Movement(float sensitivity) noexcept;

  void operator()(float x, float y) noexcept;
  void set_next_origin() noexcept;
  const MovementOffset &get() const noexcept;

private:
  const float _kSensitivity;
  MovementOffset _offset;
  float _prevX, _prevY;
  bool _prevPosesInitialized = false;
};
}

#endif
