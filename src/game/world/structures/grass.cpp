#include "game/world/blocks/block_id.hpp"
#include "game/world/structures/structures.hpp"

namespace game::structures
{
const char *Grass::name() const noexcept { return "Grass"; }
unsigned short Grass::w() const noexcept { return 1; }
unsigned short Grass::h() const noexcept { return 1; }

std::unique_ptr<BlockData[]> Grass::data() const noexcept
{
  auto data = std::make_unique<BlockData[]>(1);
  _push_block(data, {blocks::GRASS, 0, 0});
  return data;
}
}
