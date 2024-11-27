#ifndef EFE_CAMERA_HPP
#define EFE_CAMERA_HPP

#include "math/movement.hpp"
#include "math/zooming.hpp"
#include "opengl/window/mouse.hpp"
#include "opengl/window/renderer.hpp"
#include "opengl/window/window.hpp"

namespace gl
{
struct CameraSettings
{
  Window *win;
  Renderer *renderer;

  MouseButton mvButton;
  float sensitivity;
  float maxZoomIn;
  float maxZoomOut;

  float initPosX;
  float initPosY;
};

class Camera final
{
public:
  explicit Camera(const CameraSettings &settings) noexcept;

  inline const math::MovementOffset &mv() const noexcept { return _mv.get(); }
  inline const math::Zoom &zoom() const noexcept { return _zoom.get(); }

private:
  const CameraSettings _kSettings;

  bool _mvBtnActive = false;
  math::Movement _mv;
  math::Zooming _zoom;
};
}

#endif
