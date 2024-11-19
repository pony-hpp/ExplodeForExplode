#ifndef _EFE_STRUCTURES_HPP_
#define _EFE_STRUCTURES_HPP_

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
