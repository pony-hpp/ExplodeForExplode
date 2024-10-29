#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "core/logger.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader.hpp"

#include <unordered_map>

namespace gl
{
class ShaderProgram final
{
public:
  ShaderProgram() noexcept;
  ~ShaderProgram() noexcept;

  void add(const Shader &shader) noexcept;
  void link();
  void use() noexcept;

  void set_uniform(const char *name, unsigned v) noexcept;
  void set_uniform(const char *name, const float *v) noexcept;

  void set_view_matrix(const math::ViewMatrix &mat) noexcept;
  void set_projection_matrix(const math::ProjectionMatrix &mat) noexcept;

private:
  unsigned _glHandle;
  mutable std::unordered_map<const char *, int> _cachedUniformLocs;
  mutable core::Logger _logger;

  static constexpr const char *_U_VIEW_NAME       = "uView";
  static constexpr const char *_U_PROJECTION_NAME = "uProjection";

  int _get_uniform(const char *name) const noexcept;
};

struct ShaderProgramLinkException
{
};
}

#endif
