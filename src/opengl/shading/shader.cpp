#include "core/exception.hpp"
#include "core/utils.hpp"
#include "opengl/opengl.hpp"
#include "opengl/shading/shader.hpp"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <memory>

namespace gl
{
Shader::~Shader() noexcept
{
  if (_glHandle)
  {
    glDeleteShader(_glHandle);
  }
}

unsigned Shader::gl_handle() const noexcept { return _glHandle; }

void Shader::load(const char *shaderPath)
{
  _logger.set_section("Load");

  _logger.debug("Checking for correct shader source file extension");
  try
  {
    core::check_file_extension(shaderPath, src_file_extension());
  }
  catch (const core::InvalidFileExtensionException &e)
  {
    _logger.error_fmt(
      "Invalid file extension: %s required, but %s provided.",
      src_file_extension(), e.provided.c_str()
    );
    throw core::InvalidFileExtensionException();
  }

  _glHandle = glCreateShader(gl_type());

  std::ifstream f(shaderPath);
  if (!f)
  {
    _logger.error_fmt(
      "Failed to open \"%s\": %s.", shaderPath, strerror(errno)
    );
    f.close();
    throw core::FopenException();
  }

  _logger.info_fmt("Loading shader source from \"%s\"", shaderPath);

  char c;
  while ((c = f.get()) != EOF)
  {
    _src += c;
  }
  f.close();

  const char *kCSrc = _src.c_str();
  glShaderSource(_glHandle, 1, &kCSrc, nullptr);

  _logger.info("Shader source loaded.");
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

    _logger.error_fmt("Error compiling shader:\n\n%s", compileStatus.get());
    throw ShaderCompilationException();
  }

  _logger.info("Shader successfully compiled.");
}

Shader::Shader(const char *loggerModule) noexcept : _logger(loggerModule) {}

VertexShader::VertexShader() noexcept : Shader("Shader/Vertex") {}
int VertexShader::gl_type() const noexcept { return GL_VERTEX_SHADER; }
const char *VertexShader::src_file_extension() const noexcept { return "vert"; }

FragmentShader::FragmentShader() noexcept : Shader("Shader/Fragment") {}
int FragmentShader::gl_type() const noexcept { return GL_FRAGMENT_SHADER; }
const char *FragmentShader::src_file_extension() const noexcept
{
  return "frag";
}
}
