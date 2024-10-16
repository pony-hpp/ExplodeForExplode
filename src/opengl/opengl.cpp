#include "opengl/opengl.hpp"

#define REGISTER_CONST(v) \
  case v: return #v; break;

const char *gl::glfw_const_to_str(int glfwConst) noexcept {
  switch (glfwConst) {
    REGISTER_CONST(GLFW_VERSION_MAJOR)
    REGISTER_CONST(GLFW_VERSION_MINOR)
    REGISTER_CONST(GLFW_OPENGL_PROFILE)
    REGISTER_CONST(GLFW_OPENGL_CORE_PROFILE)
    REGISTER_CONST(GLFW_OPENGL_FORWARD_COMPAT)
  default: return ""; break;
  }
}

const char *gl::gl_const_to_str(int glConst) noexcept {
  switch (glConst) {
    REGISTER_CONST(GL_VERTEX_SHADER)
    REGISTER_CONST(GL_FRAGMENT_SHADER)
  default: return ""; break;
  }
}
