#ifndef _MOVEMENT_HPP_
#define _MOVEMENT_HPP_

namespace game {
struct MovementOffset {
  float x, y;
};

class Movement final {
public:
  explicit Movement(float sensitivity) noexcept;

  const MovementOffset &operator()(long long x, long long y) noexcept;
  void set_next_origin() noexcept;

private:
  const float _kSensitivity;
  MovementOffset _offset;
  long long _prevX, _prevY;
  bool _prevPosesInitialized = false;
};
}

#endif
