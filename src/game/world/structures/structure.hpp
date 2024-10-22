#ifndef _STRUCTURE_HPP_
#define _STRUCTURE_HPP_

#include "game/world/blocks/block_data.hpp"

#include <memory>

namespace game {
class Structure {
public:
  virtual ~Structure() = default;

  virtual unsigned short w() const noexcept                  = 0;
  virtual unsigned short h() const noexcept                  = 0;
  virtual std::unique_ptr<BlockData[]> data() const noexcept = 0;

  static std::unique_ptr<Structure> from_id(unsigned char id) noexcept;

protected:
  void _push_block(
    std::unique_ptr<BlockData[]> &structure, const BlockData &block
  ) const noexcept;

private:
  mutable unsigned _pushIdx = 0;
};
}

#endif
