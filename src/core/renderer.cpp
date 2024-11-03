#include "core/renderer.hpp"
#include "opengl/shading/uniforms.hpp"

namespace core
{
Renderer::Renderer(Window &win) noexcept : _win(win), _logger("Renderer") {}

unsigned short Renderer::viewport_w() const noexcept { return _viewportW; }
unsigned short Renderer::viewport_h() const noexcept { return _viewportH; }

void Renderer::init()
{
  _logger.set_section("Initialize");
  _logger.info("Initializing");

  _resize_viewport(_win.w(), _win.h());
  _win.on_resize([this](unsigned short w, unsigned short h) noexcept
  {
    _resize_viewport(w, h);
  });

  _logger.debug("Initializing GLEW");
  glewExperimental       = true;
  const int kGlewInitErr = glewInit();
  if (kGlewInitErr != 0)
  {
    const unsigned char *kErr = glewGetErrorString(kGlewInitErr);
    _logger.error_fmt("Failed to initialize renderer! %s.", kErr);
    throw RendererInitializationException {"Failed to initialize renderer"};
  }

  _logger.info_fmt("OpenGL version: %s.", glGetString(GL_VERSION));
  _logger.info("Renderer successfully initialized.");

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

  shaderProgram.set_uniform(gl::UNIFORM_VIEW, view);
  shaderProgram.set_uniform(
    gl::UNIFORM_PROJECTION, gl::math::ProjectionMatrix(_win.w(), _win.h())
  );
}

void Renderer::update_view() noexcept
{
  for (gl::ShaderProgram *shaderProgram : _shaderPrograms)
  {
    shaderProgram->set_uniform(gl::UNIFORM_VIEW, view);
  }
}

void Renderer::_resize_viewport(unsigned short w, unsigned short h) noexcept
{
  _viewportW = w;
  _viewportH = h;

  for (gl::ShaderProgram *shaderProgram : _shaderPrograms)
  {
    shaderProgram->set_uniform(
      gl::UNIFORM_PROJECTION, gl::math::ProjectionMatrix(w, h)
    );
  }
}
}
