#ifndef EFE_WINDOW_HPP
#define EFE_WINDOW_HPP

#include "core/decoders/png.hpp"
#include "core/logger.hpp"
#include "core/types.hpp"
#include "opengl/opengl.hpp" // IWYU pragma: keep
#include "opengl/window/mouse.hpp"

#include <functional>
#include <string>

namespace gl
{
class Window final
{
public:
  using ResizeCallback     = std::function<void(ushort, ushort)>;
  using CursorMoveCallback = std::function<void(int, int)>;
  using MouseClickCallback = std::function<void(MouseButton, MouseAction)>;
  using ScrollCallback     = std::function<void(bool)>;

  explicit Window(ushort w, ushort h, const char *title);
  ~Window() noexcept;

  inline GLFWwindow *gl_handle() noexcept { return _glHandle; }
  bool closed() const noexcept;
  ushort w() const noexcept;
  ushort h() const noexcept;
  ushort cursor_x() const noexcept;
  ushort cursor_y() const noexcept;

  void poll_events() const noexcept;
  void toggle_cursor_visibility() noexcept;
  void set_icon(const core::Png &icon) noexcept;

  void on_resize(const ResizeCallback &callback) noexcept;
  void on_cursor_move(const CursorMoveCallback &callback) noexcept;
  void on_mouse_click(const MouseClickCallback &callback) noexcept;
  void on_scroll(const ScrollCallback &callback) noexcept;

private:
  GLFWwindow *_glHandle = nullptr;
  bool _glfwInitialized = false;
  core::Logger _logger;

  const char *_glfw_err() const noexcept;
};

struct WindowCreationException
{
  const std::string msg;
};
}

#endif
