#ifndef EFE_STRUCTURE_HPP
#define EFE_STRUCTURE_HPP

#include "core/types.hpp"
#include "game/utils/data_2d.hpp"
#include "game/world/blocks/block_data.hpp"

#include <memory>

namespace game
{
class Structure
{
public:
  using Data = utils::Data2D<BlockData>;

  virtual ~Structure() = default;

  virtual const char *name() const noexcept = 0;
  virtual Data data() const noexcept        = 0;

  static std::unique_ptr<Structure> from_id(ubyte id) noexcept;
};
}

#endif
