#ifndef EFE_STRUCTURES_HPP
#define EFE_STRUCTURES_HPP

#include "game/world/structures/structure.hpp"

namespace game::structures
{
#define STRUCTURE(className) \
  class className final : public Structure \
  { \
  public: \
    const char *name() const noexcept override; \
    Data data() const noexcept override; \
  };

STRUCTURE(Tree)
STRUCTURE(Pond)
}

#endif
