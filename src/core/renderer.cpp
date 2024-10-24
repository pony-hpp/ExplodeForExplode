#include "core/renderer.hpp"

namespace core
{
Renderer::Renderer(Window &win, gl::ShaderProgram &shaderProgram) noexcept
  : _win(win), _shaderProgram(shaderProgram)
{
  _shaderProgram.view_matrix(view);
  win.on_resize([this](unsigned short w, unsigned short h)
  {
    _shaderProgram.projection_matrix(gl::math::ProjectionMatrix(w, h));
    _viewportW = w;
    _viewportH = h;
  });
}

unsigned short Renderer::viewport_w() const noexcept { return _viewportW; }
unsigned short Renderer::viewport_h() const noexcept { return _viewportH; }

void Renderer::draw(const IDrawable &drawable) noexcept
{
  drawable.draw(*this);
}
}
