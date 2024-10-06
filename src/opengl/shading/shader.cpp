#include "opengl/opengl.hpp"
#include "opengl/shading/shader.hpp"

#include <fstream>

namespace gl {
Shader::~Shader() noexcept {
  glDeleteShader(_glHandle);
}

void Shader::load(const char *filename) noexcept {
  _glHandle = glCreateShader(gl_type());

  std::ifstream f(filename);
  char c;
  while ((c = f.get()) != EOF) {
    _src += c;
  }
  f.close();

  const char *kCSrc = _src.c_str();
  glShaderSource(_glHandle, 1, &kCSrc, nullptr);
}

void Shader::compile() noexcept {
  glCompileShader(_glHandle);
}

unsigned Shader::gl_handle() const noexcept {
  return _glHandle;
}

int VertexShader::gl_type() const noexcept {
  return GL_VERTEX_SHADER;
}

int FragmentShader::gl_type() const noexcept {
  return GL_FRAGMENT_SHADER;
}
}
