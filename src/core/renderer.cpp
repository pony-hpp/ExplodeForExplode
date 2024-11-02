#include "core/renderer.hpp"

namespace core
{
Renderer::Renderer(Window &win) noexcept : _win(win) {}

unsigned short Renderer::viewport_w() const noexcept { return _viewportW; }
unsigned short Renderer::viewport_h() const noexcept { return _viewportH; }

void Renderer::init() noexcept
{
  update_view();
  _resize_viewport(_win.w(), _win.h());
  _win.on_resize([this](unsigned short w, unsigned short h) noexcept
  {
    _resize_viewport(w, h);
  });

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

void Renderer::draw(const IDrawable &drawable) noexcept
{
  drawable.draw(*this);
}

void Renderer::add_shader_program(gl::ShaderProgram &shaderProgram) noexcept
{
  _shaderPrograms.push_back(&shaderProgram);
}

void Renderer::update_view() noexcept
{
  for (gl::ShaderProgram *shaderProgram : _shaderPrograms)
  {
    shaderProgram->set_view_matrix(view);
  }
}

void Renderer::_resize_viewport(unsigned short w, unsigned short h) noexcept
{
  _viewportW = w;
  _viewportH = h;

  const gl::math::ProjectionMatrix kMat(w, h);
  for (gl::ShaderProgram *shaderProgram : _shaderPrograms)
  {
    shaderProgram->set_projection_matrix(kMat);
  }
}
}
