#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "opengl/math/math.hpp"
#include "opengl/shading/shader.hpp"

#include <unordered_map>

namespace gl {
class ShaderProgram final {
public:
  ShaderProgram() noexcept;
  ~ShaderProgram() noexcept;

  void add(const Shader &shader) noexcept;
  void link() noexcept;
  void use() noexcept;
  void projection(const math::Matrix &mat) noexcept;

private:
  unsigned _glHandle;
  std::unordered_map<const char *, int> _cachedUniformLocs;

  static constexpr const char *_U_PROJECTION_NAME = "uProjection";

  int _get_uniform(const char *name) noexcept;
};
}

#endif
