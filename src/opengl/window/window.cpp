#include "opengl/ctx.hpp"
#include "opengl/window/window.hpp"

#include <vector>

namespace gl
{
static std::vector<Window::ResizeCallback> _resizeCallbacks;
static std::vector<Window::CursorMoveCallback> _cursorMoveCallbacks;
static std::vector<Window::MouseClickCallback> _mouseClickCallbacks;
static std::vector<Window::ScrollCallback> _scrollCallbacks;

Window::Window(ushort w, ushort h, const char *title) : _logger("Window")
{
  _logger.set_section("Create");

  _logger.info_fmt("Creating %ux%u window with title \"%s\"", w, h, title);

  if (glfwInit() == GLFW_TRUE)
  {
    _glfwInitialized = true;
  }
  else
  {
    const char *kErr = _glfw_err();
    _logger.error_fmt("Failed to initialize GLFW: %s.", kErr);
    throw WindowCreationException {kErr};
  }

  _glHandle = glfwCreateWindow(w, h, title, nullptr, nullptr);
  if (!_glHandle)
  {
    const char *kErr = _glfw_err();
    _logger.error_fmt("Failed to create window: %s.", kErr);
    throw WindowCreationException {kErr};
  }

  _logger.debug("Creating context with these OpenGL parameters:");
  for (const auto &kParam : CTX)
  {
    const std::string kParamStr = kParam.first == GLFW_OPENGL_PROFILE
                                    ? glfw_const_to_str(kParam.second)
                                    : std::to_string(kParam.second);
    _logger.debug_fmt(
      "    %s: %s", glfw_const_to_str(kParam.first), kParamStr.c_str()
    );

    glfwWindowHint(kParam.first, kParam.second);
  }

  glfwMakeContextCurrent(_glHandle);

  _logger.info("Window successfully created.");
}

Window::~Window() noexcept
{
  if (_glHandle)
  {
    glfwDestroyWindow(_glHandle);
  }

  if (_glfwInitialized)
  {
    glfwTerminate();
  }
}

bool Window::closed() const noexcept
{
  return glfwWindowShouldClose(_glHandle);
}

ushort Window::w() const noexcept
{
  int w;
  glfwGetWindowSize(_glHandle, &w, nullptr);
  return w;
}

ushort Window::h() const noexcept
{
  int h;
  glfwGetWindowSize(_glHandle, nullptr, &h);
  return h;
}

ushort Window::cursor_x() const noexcept
{
  double x;
  glfwGetCursorPos(_glHandle, &x, nullptr);
  return x;
}

ushort Window::cursor_y() const noexcept
{
  double y;
  glfwGetCursorPos(_glHandle, nullptr, &y);
  return h() - y;
}

void Window::poll_events() const noexcept { glfwPollEvents(); }

void Window::toggle_cursor_visibility() noexcept
{
  const int kNewCursorMode =
    glfwGetInputMode(_glHandle, GLFW_CURSOR) == GLFW_CURSOR_NORMAL
      ? GLFW_CURSOR_DISABLED
      : GLFW_CURSOR_NORMAL;

  glfwSetInputMode(_glHandle, GLFW_CURSOR, kNewCursorMode);
}

void Window::set_icon(const core::Png &icon) noexcept
{
  const GLFWimage kGlfwIcon = {icon.w, icon.h, icon.data.get()};
  glfwSetWindowIcon(_glHandle, 1, &kGlfwIcon);
}

void Window::on_resize(const ResizeCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Adding resize callback");

  _resizeCallbacks.push_back(callback);
  glfwSetWindowSizeCallback(
    _glHandle,
    [](GLFWwindow *, int w, int h) noexcept
  {
    for (ResizeCallback &callback : _resizeCallbacks)
    {
      callback(w, h);
    }
  }
  );
}

void Window::on_cursor_move(const CursorMoveCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Adding cursor move callback");

  _cursorMoveCallbacks.push_back(callback);
  glfwSetCursorPosCallback(
    _glHandle,
    [](GLFWwindow *, double x, double y) noexcept
  {
    for (CursorMoveCallback &callback : _cursorMoveCallbacks)
    {
      callback(x, y);
    }
  }
  );
}

void Window::on_mouse_click(const MouseClickCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Adding mouse click callback");

  _mouseClickCallbacks.push_back(callback);
  glfwSetMouseButtonCallback(
    _glHandle,
    [](GLFWwindow *, int btn, int action, int) noexcept
  {
    for (MouseClickCallback &callback : _mouseClickCallbacks)
    {
      callback((MouseButton)btn, (MouseAction)action);
    }
  }
  );
}

void Window::on_scroll(const ScrollCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Adding mouse scroll callback");

  _scrollCallbacks.push_back(callback);
  glfwSetScrollCallback(
    _glHandle,
    [](GLFWwindow *, double, double y) noexcept
  {
    for (ScrollCallback &callback : _scrollCallbacks)
    {
      callback(y > 0.0f);
    }
  }
  );
}

const char *Window::_glfw_err() const noexcept
{
  const char *res;
  glfwGetError(&res);
  return res;
}
}
