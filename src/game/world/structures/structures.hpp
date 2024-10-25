#ifndef _STRUCTURES_HPP_
#define _STRUCTURES_HPP_

#include "game/world/structures/structure.hpp"

namespace game::structures
{
#define STRUCTURE(className) \
  class className final : public Structure \
  { \
  public: \
    unsigned short w() const noexcept override; \
    unsigned short h() const noexcept override; \
    std::unique_ptr<BlockData[]> data() const noexcept override; \
  };

STRUCTURE(Tree)
STRUCTURE(Grass)
}

#endif
