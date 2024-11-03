#include "opengl/opengl.hpp"
#include "opengl/shading/shader_program.hpp"

#include <memory>

namespace gl
{
unsigned ShaderProgram::_used;

ShaderProgram::ShaderProgram(const char *name) noexcept
  : _logger(std::string("ShaderProgram/") + name)
{
  _glHandle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() noexcept { glDeleteProgram(_glHandle); }

void ShaderProgram::add(const Shader &shader) noexcept
{
  _logger.set_section("");
  _logger.debug_fmt(
    "Adding shader with type %s", gl_const_to_str(shader.gl_type())
  );
  glAttachShader(_glHandle, shader.gl_handle());
}

void ShaderProgram::link()
{
  _logger.set_section("Link");

  _logger.info("Linking");

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

void ShaderProgram::use() noexcept
{
  glUseProgram(_glHandle);
  _used = _glHandle;
}

void ShaderProgram::set_uniform(const char *name, unsigned v) noexcept
{
  if (_used != _glHandle)
  {
    use();
  }
  glUniform1ui(_get_uniform(name), v);
}

void ShaderProgram::set_uniform(const char *name, const float *v) noexcept
{
  if (_used != _glHandle)
  {
    use();
  }
  glUniformMatrix4fv(_get_uniform(name), 1, true, v);
}

int ShaderProgram::_get_uniform(const char *name) const noexcept
{
  const auto kLoc = _cachedUniformLocs.find(name);
  if (kLoc == _cachedUniformLocs.cend())
  {
    _logger.set_section("");
    _logger.debug_fmt("Caching uniform \"%s\"", name);

    const int kUniformLoc = glGetUniformLocation(_glHandle, name);
    _cachedUniformLocs.insert({name, kUniformLoc});
    return kUniformLoc;
  }

  return kLoc->second;
}
}
