#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "core/decoders/png.hpp"
#include "core/input/mouse.hpp"
#include "core/logger.hpp"
#include "opengl/opengl.hpp" // IWYU pragma: keep

#include <functional>
#include <string>

namespace core
{
class Window final
{
public:
  using ResizeCallback = std::function<void(unsigned short, unsigned short)>;
  using CursorMoveCallback = std::function<void(int, int)>;
  using MouseClickCallback = std::function<void(mouse::Button, mouse::Action)>;
  using ScrollCallback     = std::function<void(bool)>;

  explicit Window(
    unsigned short w, unsigned short h, const char *title
  ) noexcept;
  ~Window() noexcept;

  void create();

  bool closed() const noexcept;
  unsigned short w() const noexcept;
  unsigned short h() const noexcept;

  void clear() noexcept;
  void update() noexcept;
  void poll_events() const noexcept;
  void set_bg(unsigned char r, unsigned char g, unsigned char b) noexcept;
  void toggle_cursor_visibility() noexcept;
  void set_icon(const Png &icon) noexcept;

  void on_resize(const ResizeCallback &callback) noexcept;
  void on_cursor_move(const CursorMoveCallback &callback) noexcept;
  void on_mouse_click(const MouseClickCallback &callback) noexcept;
  void on_scroll(const ScrollCallback &callback) noexcept;

private:
  const unsigned short _kInitW, _kInitH;
  const char *_kInitTitle;
  GLFWwindow *_glHandle = nullptr;
  bool _glfwInitialized = false;
  Logger _logger;

  const char *_glfw_err() const noexcept;
};

struct WindowCreationException
{
  const std::string msg;
};
}

#endif
