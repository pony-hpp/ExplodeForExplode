#include "opengl/opengl.hpp"
#include "opengl/shading/shader_program.hpp"

#include <memory>

namespace gl
{
uint ShaderProgram::_curGlHandle;

ShaderProgram::ShaderProgram(const char *name) noexcept
  : _logger(std::string("ShaderProgram/") + name)
{
  _glHandle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() noexcept { glDeleteProgram(_glHandle); }

void ShaderProgram::add(const Shader &shader) noexcept
{
  glAttachShader(_glHandle, shader.gl_handle());

  _logger.set_section("");
  _logger.debug_fmt(
    "Added shader with type %s.", gl_const_to_str(shader.gl_type())
  );
}

void ShaderProgram::link()
{
  _logger.set_section("Link");

  _logger.info("Linking shader program");

  glLinkProgram(_glHandle);

  int linkStatusStrLen;
  glGetProgramiv(_glHandle, GL_INFO_LOG_LENGTH, &linkStatusStrLen);
  if (linkStatusStrLen)
  {
    auto linkStatus = std::make_unique<char[]>(linkStatusStrLen);
    glGetProgramInfoLog(_glHandle, linkStatusStrLen, nullptr, linkStatus.get());

    _logger.error_fmt(
      "Error linking shader program:\n\n%s\n", linkStatus.get()
    );
    throw ShaderProgramLinkException();
  }

  _logger.info("Shader program successfully linked.");
}

bool ShaderProgram::used() const noexcept { return _curGlHandle == _glHandle; }

void ShaderProgram::use() noexcept
{
  if (!used())
  {
    glUseProgram(_glHandle);
    _curGlHandle = _glHandle;
  }
}

void ShaderProgram::set_uniform(const char *name, int v) noexcept
{
  use();
  glUniform1i(_get_uniform(name), v);
}

void ShaderProgram::set_uniform(const char *name, uint v) noexcept
{
  use();
  glUniform1ui(_get_uniform(name), v);
}

void ShaderProgram::set_uniform(const char *name, float v) noexcept
{
  use();
  glUniform1f(_get_uniform(name), v);
}

void ShaderProgram::set_uniform(const char *name, const float *v) noexcept
{
  use();
  glUniformMatrix4fv(_get_uniform(name), 1, true, v);
}

int ShaderProgram::_get_uniform(const char *name) const noexcept
{
  const auto kLoc = _cachedUniformLocs.find(name);
  if (kLoc == _cachedUniformLocs.cend())
  {
    const int kUniformLoc = glGetUniformLocation(_glHandle, name);
    _cachedUniformLocs.insert({name, kUniformLoc});

    _logger.set_section("");
    _logger.debug_fmt("Cached uniform \"%s\".", name);

    return kUniformLoc;
  }

  return kLoc->second;
}
}
