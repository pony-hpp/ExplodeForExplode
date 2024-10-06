#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "core/drawable.hpp"
#include "opengl/opengl.hpp" // IWYU pragma: keep

#include <functional>

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
  void draw(const IDrawable &drawable) noexcept;
  void
  on_resize(const std::function<void(unsigned short, unsigned short)> &callback
  ) noexcept;

private:
  GLFWwindow *_glHandle;
};
}

#endif
