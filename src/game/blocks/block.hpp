#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "core/drawable.hpp"

namespace game {
class Block : public core::IDrawable {
public:
  Block() noexcept;
  virtual ~Block() noexcept;

  static constexpr unsigned char SIZE = 24;

  void draw() const noexcept override;

  virtual const char *texture() const noexcept = 0;

  void set_pos(int x, int y) noexcept;
  void load_texture() noexcept;

private:
  unsigned _coordsVbo;
  unsigned _tex;
  unsigned _texCoordsVbo;
  unsigned _vao;
};
}

#endif
