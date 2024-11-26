#include "opengl/mesh.hpp"
#include "opengl/texture.hpp"

namespace gl
{
uint Mesh::_texCoordsVbo;
std::unordered_map<const core::Png *, uint> Mesh::_texUnits;
uint Mesh::_defaultTexUnit;
bool Mesh::_defaultTexUnitUsed;
ullong Mesh::_meshCount;

Mesh::Mesh() noexcept
{
  _meshCount++;

  glGenBuffers(1, &_coordsVbo);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
}

Mesh::~Mesh() noexcept
{
  _meshCount--;

  glDeleteVertexArrays(1, &_vao);

  glDeleteBuffers(1, &_coordsVbo);

  if ((_texUnits.size() || _defaultTexUnitUsed) && _meshCount == 0)
  {
    glDeleteBuffers(1, &_texCoordsVbo);

    for (auto &tex : _texUnits)
    {
      glDeleteTextures(1, &tex.second);
    }

    if (_defaultTexUnitUsed)
    {
      glDeleteTextures(1, &_defaultTexUnit);
    }
  }
}

void Mesh::update_vertices(UpdateIntensity intensity) noexcept
{
  const ullong kBufSize = vertices.size() * 2 * sizeof(float);

  glBindBuffer(GL_ARRAY_BUFFER, _coordsVbo);
  glBufferData(GL_ARRAY_BUFFER, kBufSize, vertices.data(), intensity);

  glBindVertexArray(_vao);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
}

void Mesh::load_texture(const core::Png *png, ubyte idx) noexcept
{
  if (!_curTextures.size())
  {
    glGenBuffers(1, &_texCoordsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _texCoordsVbo);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(gl::TEX_COORDS), gl::TEX_COORDS, GL_STATIC_DRAW
    );

    glBindVertexArray(_vao);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);
  }

  if (png)
  {
    if (_texUnits.find(png) != _texUnits.cend())
    {
      _curTextures.insert({idx, _texUnits[png]});
      return;
    }

    _texUnits.insert({png, 0});
    uint &tex = _texUnits[png];
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, png->w, png->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      png->data.get()
    );

    _curTextures.insert({idx, _texUnits[png]});
  }
  else
  {
    if (!_defaultTexUnitUsed)
    {
      glGenTextures(1, &_defaultTexUnit);
      glBindTexture(GL_TEXTURE_2D, _defaultTexUnit);
      glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        gl::FALLBACK_TEX
      );
    }

    _curTextures.insert({idx, _defaultTexUnit});
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Mesh::draw(DrawMode mode, uint count, uint start) const noexcept
{
  glBindVertexArray(_vao);

  for (const auto &tex : _curTextures)
  {
    glActiveTexture(GL_TEXTURE0 + tex.first);
    glBindTexture(GL_TEXTURE_2D, tex.second);
  }

  glDrawArrays(mode, start, start + count);
}
}
