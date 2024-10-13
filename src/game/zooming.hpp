#ifndef _SCALING_HPP_
#define _SCALING_HPP_

namespace game {
struct Zoom {
  float scale;
  float offsetX, offsetY;
};

class Zooming final {
public:
  Zooming(float sensitivity, float min, float max) noexcept;

  Zoom &operator()(int pointX, int pointY, bool scale) noexcept;

private:
  const float _kSensitivity, _kMin, _kMax;
  Zoom _zoom;
  bool _clamped = false;

  void _translate_to_point(int x, int y) noexcept;
};
}

#endif
