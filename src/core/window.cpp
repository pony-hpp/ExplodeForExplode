#include "core/window.hpp"
#include "opengl/ctx.hpp"

#include <cmath>

namespace core {
static Window::ResizeCallback _resizeCallback;
static Window::CursorMoveCallback _cursorMoveCallback;
static Window::MouseClickCallback _mouseClickCallback;
static Window::ScrollCallback _scrollCallback;

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

unsigned short Window::cursor_x() const noexcept {
  double x;
  glfwGetCursorPos(_glHandle, &x, nullptr);
  return fmax(0, x); // Convert negative values to 0.
}

unsigned short Window::cursor_y() const noexcept {
  int h;
  glfwGetWindowSize(_glHandle, nullptr, &h);
  double y;
  glfwGetCursorPos(_glHandle, nullptr, &y);
  return fmax(0, h - y);
}

void Window::toggle_cursor_visibility() noexcept {
  if (glfwGetInputMode(_glHandle, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
    glfwSetInputMode(_glHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(_glHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void Window::on_resize(const ResizeCallback &callback) noexcept {
  if (!_resizeCallback) {
    glfwSetWindowSizeCallback(_glHandle, [](GLFWwindow *, int w, int h) {
      glViewport(0, 0, w, h);
      _resizeCallback(w, h);
    });
  }
  _resizeCallback = callback;
}

void Window::on_cursor_move(const CursorMoveCallback &callback) noexcept {
  if (!_cursorMoveCallback) {
    glfwSetCursorPosCallback(_glHandle, [](GLFWwindow *, double x, double y) {
      _cursorMoveCallback(x, y);
    });
  }
  _cursorMoveCallback = callback;
}

void Window::on_mouse_click(const MouseClickCallback &callback) noexcept {
  if (!_mouseClickCallback) {
    glfwSetMouseButtonCallback(
      _glHandle,
      [](GLFWwindow *, int btn, int action, int) {
      _mouseClickCallback((mouse::Button)btn, (mouse::Action)action);
    }
    );
  }
  _mouseClickCallback = callback;
}

void Window::on_scroll(const ScrollCallback &callback) noexcept {
  if (!_scrollCallback) {
    glfwSetScrollCallback(_glHandle, [](GLFWwindow *, double, double y) {
      if ((char)y == 0) {
        return;
      }
      _scrollCallback(y > 0.0f);
    });
  }
  _scrollCallback = callback;
}
}
