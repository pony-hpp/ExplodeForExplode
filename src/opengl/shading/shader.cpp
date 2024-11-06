#include "core/exception.hpp"
#include "opengl/opengl.hpp"
#include "opengl/shading/shader.hpp"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <memory>

namespace gl
{
Shader::Shader(const char *name) noexcept
  : _logger(std::string("Shader/") + name)
{
}

Shader::~Shader() noexcept
{
  if (_glHandle)
  {
    glDeleteShader(_glHandle);
  }
}

unsigned Shader::gl_handle() const noexcept { return _glHandle; }

void Shader::load(const char *srcPath)
{
  _logger.set_section("Load");

  _logger.info_fmt("Loading shader source from \"%s\"", srcPath);

  std::ifstream f(srcPath);
  if (!f)
  {
    _logger.error_fmt("Failed to open \"%s\": %s.", srcPath, strerror(errno));
    f.close();
    throw core::FopenException();
  }

  char c;
  while ((c = f.get()) != EOF)
  {
    _src += c;
  }
  f.close();

  _glHandle         = glCreateShader(gl_type());
  const char *kCSrc = _src.c_str();
  glShaderSource(_glHandle, 1, &kCSrc, nullptr);

  _logger.info("Shader source successfully loaded.");
}

void Shader::compile()
{
  _logger.set_section("Compile");

  _logger.info("Compiling");

  glCompileShader(_glHandle);

  int compileStatusStrLen;
  glGetShaderiv(_glHandle, GL_INFO_LOG_LENGTH, &compileStatusStrLen);
  if (compileStatusStrLen)
  {
    auto compileStatus = std::make_unique<char[]>(compileStatusStrLen);
    glGetShaderInfoLog(
      _glHandle, compileStatusStrLen, nullptr, compileStatus.get()
    );

    _logger.error_fmt("Shader compilation error:\n\n%s\n", compileStatus.get());
    throw ShaderCompilationException();
  }

  _logger.info("Shader successfully compiled.");
}

VertexShader::VertexShader() noexcept : Shader("Vertex") {}
int VertexShader::gl_type() const noexcept { return GL_VERTEX_SHADER; }

FragmentShader::FragmentShader() noexcept : Shader("Fragment") {}
int FragmentShader::gl_type() const noexcept { return GL_FRAGMENT_SHADER; }
}
