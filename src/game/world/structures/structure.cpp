#include "game/world/structures/structure.hpp"
#include "game/world/structures/structure_id.hpp"
#include "game/world/structures/structures.hpp"

namespace game
{
std::unique_ptr<Structure> Structure::from_id(ubyte id) noexcept
{
  switch (id)
  {
  case structures::TREE: return std::make_unique<structures::Tree>();
  case structures::POND: return std::make_unique<structures::Pond>();
  default: return nullptr;
  };
}
}
