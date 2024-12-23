#ifndef EFE_CTX_HPP
#define EFE_CTX_HPP

#include "opengl/opengl.hpp" // IWYU pragma: keep

#include <utility>

namespace gl
{
constexpr std::pair<int, int> CTX[] = {
  {GLFW_VERSION_MAJOR, 3},
  {GLFW_VERSION_MINOR, 3},
  {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE},
  {GLFW_OPENGL_FORWARD_COMPAT, false}
};
}

#endif
