#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "opengl/opengl.hpp" // IWYU pragma: keep

namespace core {
class Window final {
public:
  explicit Window(
    unsigned short w, unsigned short h, const char *title
  ) noexcept;
  ~Window() noexcept;

  bool closed() const noexcept;
  void poll_events() const noexcept;
  void clear() noexcept;
  void update() noexcept;
  void set_bg(unsigned char r, unsigned char g, unsigned char b) noexcept;

private:
  GLFWwindow *_glHandle;
};
}

#endif
