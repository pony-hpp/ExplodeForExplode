#include "core/renderer.hpp"

namespace core {
Renderer::Renderer(Window &win, gl::ShaderProgram &shaderProgram) noexcept
  : _win(win), _shaderProgram(shaderProgram) {
  _shaderProgram.view(view);
  win.on_resize([this](unsigned short w, unsigned short h) {
    _viewportW = w;
    _viewportH = h;
    _shaderProgram.projection(gl::math::ProjectionMatrix(w, h));
  });
}

unsigned short Renderer::viewportW() const noexcept {
  return _viewportW;
}

unsigned short Renderer::viewportH() const noexcept {
  return _viewportH;
}

void Renderer::draw(const IDrawable &drawable) noexcept {
  drawable.draw(*this);
}
}
