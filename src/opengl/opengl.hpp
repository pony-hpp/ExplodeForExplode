#ifndef _OPENGL_HPP_
#define _OPENGL_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace gl {
extern const char *glfw_const_to_str(int glfwConst) noexcept;
extern const char *gl_const_to_str(int glConst) noexcept;
}

#endif
