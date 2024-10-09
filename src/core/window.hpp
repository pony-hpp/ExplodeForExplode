#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "core/drawable.hpp"
#include "core/input/mouse.hpp"
#include "core/sizeable.hpp"
#include "opengl/opengl.hpp" // IWYU pragma: keep

#include <functional>

namespace core {
class Window final : public ISizeable<unsigned short> {
public:
  using ResizeCallback = std::function<void(unsigned short, unsigned short)>;
  using CursorMoveCallback = std::function<void(long long, long long)>;
  using MouseClickCallback = std::function<void(mouse::Button, mouse::Action)>;

  explicit Window(
    unsigned short w, unsigned short h, const char *title
  ) noexcept;
  ~Window() noexcept;

  unsigned short w() const noexcept override;
  unsigned short h() const noexcept override;

  bool closed() const noexcept;
  void poll_events() const noexcept;
  void clear() noexcept;
  void update() noexcept;
  void set_bg(unsigned char r, unsigned char g, unsigned char b) noexcept;
  void draw(const IDrawable &drawable) noexcept;
  void toggle_cursor_visibility() noexcept;
  void set_view_pos(float x, float y) noexcept;

  void on_resize(const ResizeCallback &callback) noexcept;
  void on_cursor_move(const CursorMoveCallback &callback) noexcept;
  void on_mouse_click(const MouseClickCallback &callback) noexcept;

private:
  GLFWwindow *_glHandle;
  float _viewPosX = 0.0f, _viewPosY = 0.0f;
};
}

#endif
