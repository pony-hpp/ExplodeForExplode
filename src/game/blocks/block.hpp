#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "core/drawable.hpp"
#include "game/blocks/block_data.hpp"
#include "game/blocks/block_id.hpp"

#include <memory>

namespace game {
class Block : public core::IDrawable {
public:
  Block() noexcept;
  virtual ~Block() noexcept;

  static constexpr unsigned char SIZE = 24;

  void draw() const noexcept override;

  static std::unique_ptr<Block> from_id(blocks::BlockId id) noexcept;
  static std::unique_ptr<Block> from_data(const BlockData &data) noexcept;

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
