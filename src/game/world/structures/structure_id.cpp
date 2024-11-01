#include "game/world/structures/structure_id.hpp"

using namespace game::structures;

#define REGISTER_STRUCTURE_ID(v) \
  case v: return #v; break;

const char *game::structures::id_to_str(unsigned char id) noexcept
{
  switch (id)
  {
    REGISTER_STRUCTURE_ID(TREE)
    REGISTER_STRUCTURE_ID(GRASS)
  default: return "";
  }
}
