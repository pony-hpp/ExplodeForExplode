#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "core/drawable.hpp"
#include "core/window.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader_program.hpp"

#include <vector>

namespace core
{
class Renderer final
{
public:
  explicit Renderer(Window &win) noexcept;

  gl::math::ViewMatrix view;

  unsigned short viewport_w() const noexcept;
  unsigned short viewport_h() const noexcept;

  void init() noexcept;
  void draw(const IDrawable &drawable) noexcept;
  void add_shader_program(gl::ShaderProgram &shaderProgram) noexcept;
  void update_view() noexcept;

private:
  Window &_win;
  std::vector<gl::ShaderProgram *> _shaderPrograms;
  unsigned short _viewportW, _viewportH;

  void _resize_viewport(unsigned short w, unsigned short h) noexcept;
};
}

#endif
