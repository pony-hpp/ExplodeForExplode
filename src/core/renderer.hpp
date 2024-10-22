#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "core/drawable.hpp"
#include "core/window.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader_program.hpp"

namespace core {
class Renderer final {
public:
  Renderer(Window &win, gl::ShaderProgram &shaderProgram) noexcept;

  gl::math::ViewMatrix view;

  unsigned short viewport_w() const noexcept;
  unsigned short viewport_h() const noexcept;
  void draw(const IDrawable &drawable) noexcept;

private:
  Window &_win;
  gl::ShaderProgram &_shaderProgram;
  unsigned short _viewportW, _viewportH;
};
}

#endif
