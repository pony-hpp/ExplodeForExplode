#include "core/window.hpp"
#include "opengl/ctx.hpp"

namespace core
{
static Window::ResizeCallback _resizeCallback;
static Window::CursorMoveCallback _cursorMoveCallback;
static Window::MouseClickCallback _mouseClickCallback;
static Window::ScrollCallback _scrollCallback;

Window::Window(unsigned short w, unsigned short h, const char *title) noexcept
  : _kInitW(w), _kInitH(h), _kInitTitle(title), _logger("Window")
{
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

void Window::create()
{
  _logger.set_section("Create");

  _logger.info_fmt(
    "Creating %ux%u window with title \"%s\"", _kInitW, _kInitH, _kInitTitle
  );

  if (glfwInit() == 1)
  {
    _glfwInitialized = true;
  }
  else
  {
    const char *kErr = _glfw_err();
    _logger.error_fmt("Failed to initialize GLFW! %s.", kErr);
    throw WindowCreationException {kErr};
  }

  _glHandle = glfwCreateWindow(_kInitW, _kInitH, _kInitTitle, nullptr, nullptr);

  if (!_glHandle)
  {
    const char *kErr = _glfw_err();
    _logger.error_fmt("Failed to create window! %s.", kErr);
    throw WindowCreationException {kErr};
  }

  _logger.debug("Creating context");
  _logger.debug("Received these OpenGL initialization parameters:");
  for (const auto &param : gl::CTX)
  {
    if (param.first == GLFW_OPENGL_PROFILE)
    {
      _logger.debug_fmt(
        "    %s: %s", gl::glfw_const_to_str(param.first),
        gl::glfw_const_to_str(param.second)
      );
    }
    else
    {
      _logger.debug_fmt(
        "    %s: %i", gl::glfw_const_to_str(param.first), param.second
      );
    }

    glfwWindowHint(param.first, param.second);
  }

  glfwMakeContextCurrent(_glHandle);

  _logger.info("Window successfully created.");
}

bool Window::closed() const noexcept
{
  return glfwWindowShouldClose(_glHandle);
}

unsigned short Window::w() const noexcept
{
  int w;
  glfwGetWindowSize(_glHandle, &w, nullptr);
  return w;
}

unsigned short Window::h() const noexcept
{
  int h;
  glfwGetWindowSize(_glHandle, nullptr, &h);
  return h;
}

unsigned short Window::cursor_x() const noexcept
{
  double x;
  glfwGetCursorPos(_glHandle, &x, nullptr);
  return x;
}

unsigned short Window::cursor_y() const noexcept
{
  double y;
  glfwGetCursorPos(_glHandle, nullptr, &y);
  return h() - y;
}

void Window::clear() noexcept { glClear(GL_COLOR_BUFFER_BIT); }

void Window::update() noexcept { glfwSwapBuffers(_glHandle); }

void Window::poll_events() const noexcept { glfwPollEvents(); }

void Window::set_bg(unsigned char r, unsigned char g, unsigned char b) noexcept
{
  _logger.set_section("");
  _logger.info_fmt(
    "Setting window background color, where R=%u, G=%u, B=%u", r, g, b
  );
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}

void Window::toggle_cursor_visibility() noexcept
{
  if (glfwGetInputMode(_glHandle, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
  {
    glfwSetInputMode(_glHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  else
  {
    glfwSetInputMode(_glHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void Window::set_icon(const Png &icon) noexcept
{
  GLFWimage iconImg = {icon.w, icon.h, icon.data.get()};
  glfwSetWindowIcon(_glHandle, 1, &iconImg);
}

void Window::on_resize(const ResizeCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Setting resize callback");

  _resizeCallback = callback;
  glfwSetWindowSizeCallback(
    _glHandle,
    [](GLFWwindow *, int w, int h) noexcept
  {
    glViewport(0, 0, w, h);
    _resizeCallback(w, h);
  }
  );
}

void Window::on_cursor_move(const CursorMoveCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Setting cursor movement callback");

  _cursorMoveCallback = callback;
  glfwSetCursorPosCallback(
    _glHandle,
    [](GLFWwindow *, double x, double y) noexcept
  {
    _cursorMoveCallback(x, y);
  }
  );
}

void Window::on_mouse_click(const MouseClickCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Setting mouse click callback");

  _mouseClickCallback = callback;
  glfwSetMouseButtonCallback(
    _glHandle,
    [](GLFWwindow *, int btn, int action, int) noexcept
  {
    _mouseClickCallback((mouse::Button)btn, (mouse::Action)action);
  }
  );
}

void Window::on_scroll(const ScrollCallback &callback) noexcept
{
  _logger.set_section("");
  _logger.debug("Setting mouse scroll callback");

  _scrollCallback = callback;
  glfwSetScrollCallback(
    _glHandle,
    [](GLFWwindow *, double, double y) noexcept
  {
    if ((char)y == 0)
    {
      return;
    }
    _scrollCallback(y > 0.0f);
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
