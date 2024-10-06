#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "core/drawable.hpp"

namespace game {
class Block final : public core::IDrawable {
public:
  Block() noexcept;
  ~Block() noexcept;

  static constexpr unsigned char SIZE = 24;

  void draw() const noexcept override;

  void set_pos(int x, int y) noexcept;

private:
  unsigned _coordsVbo;
  unsigned _vao;
};
}

#endif
