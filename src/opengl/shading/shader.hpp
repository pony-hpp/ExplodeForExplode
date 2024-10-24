#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "core/logger.hpp"

#include <string>

namespace gl
{
class Shader
{
public:
  virtual ~Shader() noexcept;

  virtual int gl_type() const noexcept                    = 0;
  virtual const char *src_file_extension() const noexcept = 0;

  void load(const char *shaderPath);
  void compile();
  unsigned gl_handle() const noexcept;

protected:
  explicit Shader(const char *loggerModule) noexcept;

private:
  unsigned _glHandle = 0;
  std::string _src;
  core::Logger _logger;
};

#define SHADER(className) \
  class className final : public Shader \
  { \
  public: \
    className() noexcept; \
\
    int gl_type() const noexcept override; \
    const char *src_file_extension() const noexcept override; \
  };

SHADER(VertexShader)
SHADER(FragmentShader)

struct ShaderCompilationException
{
};
}

#endif
