#ifndef _ZOOMING_HPP_
#define _ZOOMING_HPP_

namespace game
{
struct Zoom
{
  float scale;
  float offsetX, offsetY;
};

class Zooming final
{
public:
  explicit Zooming(float sensitivity, float min, float max) noexcept;

  void operator()(bool scale, float x, float y) noexcept;
  const Zoom &get() const noexcept;

private:
  const float _kSensitivity, _kMin, _kMax;
  Zoom _zoom;
  bool _clamped;

  void _translate_to_point(float x, float y) noexcept;
};
}

#endif
