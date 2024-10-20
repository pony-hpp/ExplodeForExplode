#include "game/world_generators/structures/structure.hpp"
#include "game/world_generators/structures/structure_id.hpp"
#include "game/world_generators/structures/structures.hpp"

namespace game {
std::unique_ptr<Structure> Structure::from_id(unsigned char id) noexcept {
  switch (id) {
  case structures::TREE: return std::make_unique<structures::Tree>(); break;
  default: return nullptr;
  };
}

void Structure::_push_block(
  std::unique_ptr<BlockData[]> &structure, const BlockData &block
) const noexcept {
  structure[_pushIdx]         = block;
  structure[_pushIdx].enabled = true;
  _pushIdx++;
}
}
