#include "opengl/shading/uniforms.hpp"
#include "opengl/window/renderer.hpp"

namespace gl
{
Renderer::Renderer(Window &win) : _win(win), _drawer(*this), _logger("Renderer")
{
  _logger.set_section("Create");

  _logger.info("Creating renderer");

  _resize_viewport(win.w(), win.h());
  win.on_resize([this](ushort w, ushort h) noexcept { _resize_viewport(w, h); }
  );

  _logger.debug("Initializing GLEW");

  glewExperimental       = true;
  const int kGlewInitErr = glewInit();
  if (kGlewInitErr != GLEW_OK)
  {
    const unsigned char *kErr = glewGetErrorString(kGlewInitErr);
    _logger.error_fmt("Failed to initialize renderer: %s.", kErr);
    throw RendererInitializationException {(const char *)kErr};
  }

  _logger.info_fmt("OpenGL version: %s.", glGetString(GL_VERSION));

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  _logger.info("Renderer successfully created.");
}

void Renderer::clear() noexcept { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::clear_color(ubyte r, ubyte g, ubyte b) noexcept
{
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

  _logger.set_section("");
  _logger.info_fmt("Setted background color, where R=%i, G=%i, B=%i.", r, g, b);
}

void Renderer::update() noexcept { glfwSwapBuffers(_win.gl_handle()); }

void Renderer::add_shader_program(ShaderProgram &shaderProgram) noexcept
{
  _shaderPrograms.push_back(&shaderProgram);

  shaderProgram.set_uniform(UNIFORM_VIEW, (float *)view);
  shaderProgram.set_uniform(
    UNIFORM_PROJECTION, (float *)math::ProjectionMatrix(_win.w(), _win.h())
  );
}

void Renderer::update_view() noexcept
{
  for (ShaderProgram *shaderProgram : _shaderPrograms)
  {
    shaderProgram->set_uniform(UNIFORM_VIEW, (float *)view);
  }
}

void Renderer::draw(const ISingleDrawable &drawable) noexcept
{
  if (_visible_in_viewport(drawable))
  {
    drawable.draw();
  }
}

void Renderer::draw(const ICompositeDrawable &drawable) noexcept
{
  if (_visible_in_viewport(drawable))
  {
    drawable.draw(_drawer);
  }
}

bool Renderer::_visible_in_viewport(const IDrawable &drawable) const noexcept
{
  if (drawable.always_draw())
  {
    return true;
  }

  const float kPointX = view.get_offset_x() + drawable.x() * view.get_scale(),
              kPointY = view.get_offset_y() + drawable.y() * view.get_scale();

  return kPointX > (int)-drawable.w() * view.get_scale() &&
         kPointX <= _viewportW &&
         kPointY > (int)-drawable.h() * view.get_scale() &&
         kPointY <= _viewportH;
}

void Renderer::_resize_viewport(ushort w, ushort h) noexcept
{
  _viewportW = w;
  _viewportH = h;

  glViewport(0, 0, w, h);

  for (ShaderProgram *shaderProgram : _shaderPrograms)
  {
    shaderProgram->set_uniform(
      UNIFORM_PROJECTION, (float *)math::ProjectionMatrix(w, h)
    );
  }
}

void Drawer::draw(const ISingleDrawable &drawable) noexcept
{
  _renderer.draw(drawable);
}
}
