#ifndef EFE_MOUSE_HPP
#define EFE_MOUSE_HPP

#include "opengl/opengl.hpp" // IWYU pragma: keep

namespace gl
{
enum MouseButton
{
  MOUSE_BUTTON_LEFT  = GLFW_MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_MID   = GLFW_MOUSE_BUTTON_MIDDLE
};

enum MouseAction
{
  MOUSE_ACTION_PRESS   = GLFW_PRESS,
  MOUSE_ACTION_RELEASE = GLFW_RELEASE
};
}

#endif
