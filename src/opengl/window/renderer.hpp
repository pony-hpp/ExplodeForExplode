#ifndef EFE_RENDERER_HPP
#define EFE_RENDERER_HPP

#include "core/logger.hpp"
#include "core/types.hpp"
#include "math/matrix/matrix.hpp"
#include "opengl/drawable.hpp"
#include "opengl/shading/shader_program.hpp"
#include "opengl/window/window.hpp"

#include <string>
#include <vector>

namespace gl
{
class Renderer;

// Encapsulates `Renderer` to allow drawing only.
class Drawer final
{
public:
  inline explicit Drawer(Renderer &renderer) noexcept : _renderer(renderer) {};

  void draw(const ISingleDrawable &drawable) noexcept;

private:
  Renderer &_renderer;
};
class Renderer final
{
public:
  explicit Renderer(Window &win);

  math::ViewMatrix view;

  inline ushort viewport_w() const noexcept { return _viewportW; }
  inline ushort viewport_h() const noexcept { return _viewportH; }

  void clear() noexcept;
  void clear_color(ubyte r, ubyte g, ubyte b) noexcept;
  void update() noexcept;
  void add_shader_program(ShaderProgram &shaderProgram) noexcept;
  void update_view() noexcept;

  void draw(const ISingleDrawable &drawable) noexcept;
  void draw(const ICompositeDrawable &drawable) noexcept;

private:
  Window &_win;
  ushort _viewportW, _viewportH;
  std::vector<ShaderProgram *> _shaderPrograms;
  Drawer _drawer;
  core::Logger _logger;

  bool _visible_in_viewport(const IDrawable &drawable) const noexcept;
  void _resize_viewport(ushort w, ushort h) noexcept;
};

struct RendererInitializationException
{
  const std::string msg;
};
}

#endif
