#include "opengl/mesh.hpp"
#include "opengl/texture.hpp"

namespace gl
{
Mesh::Mesh() noexcept
{
  glGenBuffers(1, &_coordsVbo);
  glGenBuffers(1, &_texCoordsVbo);
  glGenTextures(1, &_tex);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Mesh::~Mesh() noexcept
{
  glDeleteBuffers(1, &_coordsVbo);
  glDeleteBuffers(1, &_texCoordsVbo);
  glDeleteTextures(1, &_tex);
  glDeleteVertexArrays(1, &_vao);
}

void Mesh::update_vertices(UpdateIntensity intensity) noexcept
{
  const ullong kBufSize = vertices.size() * 2 * sizeof(float);

  glBindBuffer(GL_ARRAY_BUFFER, _coordsVbo);
  glBufferData(GL_ARRAY_BUFFER, kBufSize, vertices.data(), intensity);

  glBindVertexArray(_vao);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
}

void Mesh::load_texture(const core::Png *png) noexcept
{
  glBindBuffer(GL_ARRAY_BUFFER, _texCoordsVbo);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(gl::TEX_COORDS), gl::TEX_COORDS, GL_STATIC_DRAW
  );

  glBindVertexArray(_vao);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

  glBindTexture(GL_TEXTURE_2D, _tex);
  if (png)
  {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, png->w, png->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      png->data.get()
    );
  }
  else
  {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      gl::FALLBACK_TEX
    );
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Mesh::draw(DrawMode mode, uint count, uint start) const noexcept
{
  glBindVertexArray(_vao);
  glBindTexture(GL_TEXTURE_2D, _tex);
  glDrawArrays(mode, start, start + count);
}
}
