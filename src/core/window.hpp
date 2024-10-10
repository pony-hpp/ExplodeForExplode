#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "core/input/mouse.hpp"
#include "opengl/opengl.hpp" // IWYU pragma: keep

#include <functional>

namespace core {
class Window final {
public:
  using ResizeCallback = std::function<void(unsigned short, unsigned short)>;
  using CursorMoveCallback = std::function<void(long long, long long)>;
  using MouseClickCallback = std::function<void(mouse::Button, mouse::Action)>;

  explicit Window(
    unsigned short w, unsigned short h, const char *title
  ) noexcept;
  ~Window() noexcept;

  bool closed() const noexcept;
  void poll_events() const noexcept;
  void clear() noexcept;
  void update() noexcept;
  void set_bg(unsigned char r, unsigned char g, unsigned char b) noexcept;
  void toggle_cursor_visibility() noexcept;

  void on_resize(const ResizeCallback &callback) noexcept;
  void on_cursor_move(const CursorMoveCallback &callback) noexcept;
  void on_mouse_click(const MouseClickCallback &callback) noexcept;

private:
  GLFWwindow *_glHandle;
};
}

#endif
