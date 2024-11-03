#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "core/logger.hpp"
#include "opengl/shading/shader.hpp"

#include <unordered_map>

namespace gl
{
class ShaderProgram final
{
public:
  explicit ShaderProgram(const char *name) noexcept;
  ~ShaderProgram() noexcept;

  void add(const Shader &shader) noexcept;
  void link();
  void use() noexcept;

  void set_uniform(const char *name, unsigned v) noexcept;
  void set_uniform(const char *name, const float *v) noexcept;

private:
  static unsigned _used;

  unsigned _glHandle;
  mutable std::unordered_map<const char *, int> _cachedUniformLocs;
  mutable core::Logger _logger;

  int _get_uniform(const char *name) const noexcept;
};

struct ShaderProgramLinkException
{
};
}

#endif
