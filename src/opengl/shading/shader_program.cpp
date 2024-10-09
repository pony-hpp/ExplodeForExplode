#include "opengl/opengl.hpp"
#include "opengl/shading/shader_program.hpp"

namespace gl {
ShaderProgram::ShaderProgram() noexcept {
  _glHandle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() noexcept {
  glDeleteProgram(_glHandle);
}

void ShaderProgram::add(const Shader &shader) noexcept {
  glAttachShader(_glHandle, shader.gl_handle());
}

void ShaderProgram::link() noexcept {
  glLinkProgram(_glHandle);
}

void ShaderProgram::use() noexcept {
  glUseProgram(_glHandle);
}

void ShaderProgram::view(const math::ViewMatrix &mat) noexcept {
  glUniformMatrix4fv(_get_uniform(_U_VIEW_NAME), 1, true, mat);
}

void ShaderProgram::projection(const math::ProjectionMatrix &mat) noexcept {
  glUniformMatrix4fv(_get_uniform(_U_PROJECTION_NAME), 1, true, mat);
}

int ShaderProgram::_get_uniform(const char *name) noexcept {
  const auto &kLoc = _cachedUniformLocs.find(name);
  if (kLoc == _cachedUniformLocs.end()) {
    _cachedUniformLocs.insert({name, glGetUniformLocation(_glHandle, name)});
    return _cachedUniformLocs.find(name)->second;
  }
  return kLoc->second;
}
}
