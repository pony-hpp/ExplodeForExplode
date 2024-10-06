#ifndef _CTX_HPP_
#define _CTX_HPP_

#include "opengl/opengl.hpp" // IWYU pragma: keep

#include <unordered_map>

namespace gl {
const std::unordered_map<int, int> CTX = {
  {GLFW_VERSION_MAJOR, 3},
  {GLFW_VERSION_MINOR, 3},
  {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE},
  {GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE}
};
}

#endif
