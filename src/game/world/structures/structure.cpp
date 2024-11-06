#include "game/world/structures/structure.hpp"
#include "game/world/structures/structure_id.hpp"
#include "game/world/structures/structures.hpp"

namespace game
{
std::unique_ptr<Structure> Structure::from_id(unsigned char id) noexcept
{
  switch (id)
  {
  case structures::TREE: return std::make_unique<structures::Tree>(); break;
  case structures::POND: return std::make_unique<structures::Pond>(); break;
  case structures::GRASS: return std::make_unique<structures::Grass>(); break;
  default: return nullptr;
  };
}

void Structure::_push_block(
  std::unique_ptr<BlockData[]> &data, const BlockData &block
) const noexcept
{
  data[_pushIdx]         = block;
  data[_pushIdx].enabled = true;
  _pushIdx++;
}
}
