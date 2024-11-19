#ifndef _EFE_MESH_HPP_
#define _EFE_MESH_HPP_

#include "core/decoders/png.hpp"
#include "core/types.hpp"
#include "opengl/opengl.hpp"

#include <vector>

namespace gl
{
enum UpdateIntensity
{
  UPD_INTENSITY_NONE = GL_STATIC_DRAW,
  UPD_INTENSITY_NORM = GL_DYNAMIC_DRAW,
  UPD_INTENSITY_HIGH = GL_STREAM_DRAW
};

enum DrawMode
{
  DRAW_MODE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP
};

struct Vertex
{
  float x, y;
};

class Mesh final
{
public:
  Mesh() noexcept;
  ~Mesh() noexcept;

  std::vector<Vertex> vertices;

  void update_vertices(UpdateIntensity intensity) noexcept;
  void load_texture(const core::Png *png = nullptr) noexcept;

  void draw(DrawMode mode, uint count, uint start = 0) const noexcept;

private:
  uint _coordsVbo, _vao;
  uint _texCoordsVbo, _tex;
};
};

#endif
