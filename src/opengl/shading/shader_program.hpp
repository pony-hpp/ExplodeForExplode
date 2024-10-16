#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "core/logger.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader.hpp"

#include <unordered_map>

namespace gl {
struct ShaderProgramLinkException {};

class ShaderProgram final {
public:
  ShaderProgram() noexcept;
  ~ShaderProgram() noexcept;

  void add(const Shader &shader) noexcept;
  void link();
  void use() noexcept;
  void view(const math::ViewMatrix &mat) noexcept;
  void projection(const math::ProjectionMatrix &mat) noexcept;

private:
  unsigned _glHandle;
  std::unordered_map<const char *, int> _cachedUniformLocs;
  core::Logger _logger;

  static constexpr const char *_U_VIEW_NAME       = "uView";
  static constexpr const char *_U_PROJECTION_NAME = "uProjection";

  int _get_uniform(const char *name) noexcept;
};
}

#endif
