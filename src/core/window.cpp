#include "core/window.hpp"
#include "opengl/ctx.hpp"

static std::function<void(unsigned short, unsigned short)> _resizeCallback;

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

void Window::draw(const IDrawable &drawable) noexcept {
  drawable.draw();
}

void Window::on_resize(
  const std::function<void(unsigned short, unsigned short)> &callback
) noexcept {
  if (!_resizeCallback) {
    glfwSetWindowSizeCallback(_glHandle, [](GLFWwindow *, int w, int h) {
      glViewport(0, 0, w, h);
      _resizeCallback(w, h);
    });
  }
  _resizeCallback = callback;
}
}
