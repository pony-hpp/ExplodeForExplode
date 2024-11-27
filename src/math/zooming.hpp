#ifndef EFE_ZOOMING_HPP
#define EFE_ZOOMING_HPP

namespace math
{
struct Zoom
{
  float scale;
  float offsetX, offsetY;
};

class Zooming final
{
public:
  inline explicit Zooming(float sensitivity, float maxIn, float maxOut) noexcept
    : _kSensitivity(sensitivity), _kMaxIn(maxIn), _kMaxOut(maxOut),
      _zoom({1.0f, 0.0f, 0.0f})
  {}

  void operator()(bool scale, float x, float y) noexcept;
  inline const Zoom &get() const noexcept { return _zoom; }

private:
  const float _kSensitivity, _kMaxIn, _kMaxOut;
  Zoom _zoom;

  bool _clamped;

  void _translate_to_point(float x, float y) noexcept;
};
}

#endif
