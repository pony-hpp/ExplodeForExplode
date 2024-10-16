#include "opengl/opengl.hpp"
#include "opengl/shading/shader_program.hpp"

#include <memory>

namespace gl {
ShaderProgram::ShaderProgram() noexcept : _logger("ShaderProgram") {
  _glHandle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() noexcept {
  glDeleteProgram(_glHandle);
}

void ShaderProgram::add(const Shader &shader) noexcept {
  _logger.set_section("");
  _logger.debug_fmt(
    "Adding shader with type %s", gl::gl_const_to_str(shader.gl_type())
  );
  glAttachShader(_glHandle, shader.gl_handle());
}

void ShaderProgram::link() {
  _logger.set_section("Link");

  _logger.info("Linking");

  glLinkProgram(_glHandle);
  int linkStatusStrLen;
  glGetProgramiv(_glHandle, GL_INFO_LOG_LENGTH, &linkStatusStrLen);
  if (linkStatusStrLen) {
    std::unique_ptr<char[]> linkStatus =
      std::make_unique<char[]>(linkStatusStrLen);
    glGetProgramInfoLog(_glHandle, linkStatusStrLen, nullptr, linkStatus.get());
    _logger.error_fmt(
      "Error linking shader program:\n\n%s\n", linkStatus.get()
    );
    throw ShaderProgramLinkException();
  }

  _logger.info("Shader program successfully linked.");
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
  if (kLoc == _cachedUniformLocs.cend()) {
    _logger.set_section("");
    _logger.debug_fmt("Caching uniform \"%s\"", name);
    _cachedUniformLocs.insert({name, glGetUniformLocation(_glHandle, name)});
    return _cachedUniformLocs.find(name)->second;
  }
  return kLoc->second;
}
}
