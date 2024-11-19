#ifndef _EFE_SHADER_HPP_
#define _EFE_SHADER_HPP_

#include "core/logger.hpp"
#include "core/types.hpp"

#include <string>

namespace gl
{
class Shader
{
public:
  virtual ~Shader() noexcept;

  virtual int gl_type() const noexcept = 0;

  inline uint gl_handle() const noexcept { return _glHandle; }
  void load(const char *srcPath);
  void compile();

protected:
  inline explicit Shader(const char *name) noexcept
    : _logger(std::string("Shader/") + name)
  {}

private:
  uint _glHandle = 0;
  std::string _src;
  core::Logger _logger;
};

struct ShaderCompilationException
{};

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
}

#endif
