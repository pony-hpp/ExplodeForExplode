#ifndef _MOUSE_HPP_
#define _MOUSE_HPP_

#include "opengl/opengl.hpp" // IWYU pragma: keep

namespace core::mouse {
enum Button : unsigned char {
  BUTTON_LEFT  = GLFW_MOUSE_BUTTON_LEFT,
  BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
  BUTTON_MID   = GLFW_MOUSE_BUTTON_MIDDLE
};

enum Action : bool {
  ACTION_PRESS   = GLFW_PRESS,
  ACTION_RELEASE = GLFW_RELEASE
};
}

#endif
