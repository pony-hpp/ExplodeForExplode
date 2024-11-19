#include "opengl/window/camera.hpp"

namespace gl
{
Camera::Camera(const CameraSettings &settings) noexcept
  : _kSettings(settings), _mv(settings.sensitivity),
    _zoom(settings.sensitivity / 10.0f, settings.maxZoomIn, settings.maxZoomOut)
{
  const float kX = settings.initPosX / settings.sensitivity -
                   settings.renderer->viewport_w() / 2.0f,
              kY = settings.initPosY / settings.sensitivity +
                   settings.renderer->viewport_h() / 2.0f;
  _mv(kX, kY);
  _mv(0.0f, 0.0f);
  _mv.reset_origin();
  settings.renderer->view.set_offset(_mv.get().x, _mv.get().y);
  settings.renderer->update_view();

  settings.win->on_mouse_click(
    [this](MouseButton btn, MouseAction action) noexcept
  {
    if (btn == _kSettings.mvButton)
    {
      _kSettings.win->toggle_cursor_visibility();

      _mvBtnActive = action == MOUSE_ACTION_PRESS;

      if (action == MOUSE_ACTION_RELEASE)
      {
        _mv.reset_origin();
      }
    }
  }
  );

  settings.win->on_cursor_move([this](int x, int y) noexcept
  {
    if (!_mvBtnActive)
    {
      return;
    }

    _mv(x / _zoom.get().scale, y / _zoom.get().scale);
    _kSettings.renderer->view.set_offset(
      _mv.get().x * _zoom.get().scale + _zoom.get().offsetX,
      _mv.get().y * _zoom.get().scale + _zoom.get().offsetY
    );

    _kSettings.renderer->update_view();
  });

  settings.win->on_scroll([this](bool up) noexcept
  {
    if (_mvBtnActive) // Prevent zooming while moving
    {
      return;
    }

    _zoom(
      up, _kSettings.renderer->viewport_w() / 2.0f,
      _kSettings.renderer->viewport_h() / 2.0f
    );

    _kSettings.renderer->view.set_scale(_zoom.get().scale);
    _kSettings.renderer->view.set_offset(
      _mv.get().x * _zoom.get().scale + _zoom.get().offsetX,
      _mv.get().y * _zoom.get().scale + _zoom.get().offsetY
    );

    _kSettings.renderer->update_view();
  });
}
}
