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

  virtual int gl_type() const noexcept = 0;

  unsigned gl_handle() const noexcept;
  void load(const char *srcPath);
  void compile();

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
  };

SHADER(VertexShader)
SHADER(FragmentShader)

struct ShaderCompilationException
{
};
}

#endif
