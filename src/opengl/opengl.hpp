#ifndef EFE_OPENGL_HPP
#define EFE_OPENGL_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define REGISTER_CONST(v) \
  case v: return #v; break;

namespace gl
{
constexpr const char *glfw_const_to_str(int glfwConst)
{
  switch (glfwConst)
  {
    REGISTER_CONST(GLFW_VERSION_MAJOR)
    REGISTER_CONST(GLFW_VERSION_MINOR)
    REGISTER_CONST(GLFW_OPENGL_PROFILE)
    REGISTER_CONST(GLFW_OPENGL_CORE_PROFILE)
    REGISTER_CONST(GLFW_OPENGL_FORWARD_COMPAT)
  default: return "";
  }
}

constexpr const char *gl_const_to_str(int glConst)
{
  switch (glConst)
  {
    REGISTER_CONST(GL_VERTEX_SHADER)
    REGISTER_CONST(GL_FRAGMENT_SHADER)
  default: return "";
  }
}
}

#endif
