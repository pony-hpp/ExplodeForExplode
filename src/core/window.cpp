#include "core/window.hpp"
#include "opengl/ctx.hpp"

namespace core {
Window::Window(unsigned short w, unsigned short h, const char *title) noexcept {
  glfwInit();

  _glHandle = glfwCreateWindow(w, h, title, nullptr, nullptr);
  for (const std::pair<int, int> &hint : gl::CTX) {
    glfwWindowHint(hint.first, hint.second);
  }
  glfwMakeContextCurrent(_glHandle);

  glewExperimental = true;
  glewInit();
}

Window::~Window() noexcept {
  glfwDestroyWindow(_glHandle);
  glfwTerminate();
}

bool Window::closed() const noexcept {
  return glfwWindowShouldClose(_glHandle);
}

void Window::poll_events() const noexcept {
  glfwPollEvents();
}

void Window::clear() noexcept {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update() noexcept {
  glfwSwapBuffers(_glHandle);
}

void Window::set_bg(
  unsigned char r, unsigned char g, unsigned char b
) noexcept {
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}
}
