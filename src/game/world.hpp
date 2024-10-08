#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "blocks/block.hpp"
#include "core/drawable.hpp"
#include "game/world_generators/world_data.hpp"

namespace game {
class World final : public core::IDrawable {
public:
  World(const WorldData &data) noexcept;
  ~World() noexcept;

  void draw() const noexcept override;

private:
  std::unique_ptr<Block> **_data;
  const unsigned _kW;
  const unsigned short _kH;
};
}

#endif
