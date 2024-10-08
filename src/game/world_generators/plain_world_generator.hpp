#ifndef _PLAIN_WORLD_GENERATOR_HPP_
#define _PLAIN_WORLD_GENERATOR_HPP_

#include "game/blocks/block_id.hpp"
#include "game/world_generators/world_data.hpp"

#include <vector>

namespace game {
struct BlocksLayer {
  blocks::BlockId block;
  unsigned short h;
};

class PlainWorldGeneratorSettings final {
public:
  unsigned w;
  std::vector<BlocksLayer> layers;

  unsigned short h() const noexcept;

private:
  mutable unsigned short _cachedH = 0;
};

class PlainWorldGenerator final {
public:
  explicit PlainWorldGenerator(const PlainWorldGeneratorSettings &settings
  ) noexcept;

  WorldData &operator()() noexcept;

private:
  const PlainWorldGeneratorSettings &_kSettings;
  WorldData _world;
};
}

#endif
