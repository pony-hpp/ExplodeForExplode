#ifndef EFE_PLAIN_WORLD_GENERATOR_HPP
#define EFE_PLAIN_WORLD_GENERATOR_HPP

#include "core/logger.hpp"
#include "core/types.hpp"
#include "game/world/generators/structures_generator.hpp"
#include "game/world/world.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace game
{
class PlainWorldGeneratorSettings final
{
public:
  using BlockLayer = std::pair<blocks::BlockId, ushort>;

  uint w;
  std::vector<BlockLayer> layers;
  std::vector<BlockData> overridenBlocks;
  const StructuresGenerator *structuresGenerator = nullptr;

  ushort h() const noexcept;
  uint extra_blocks() const noexcept;

private:
  mutable ushort _cachedH = 0;
};

class PlainWorldGenerator final
{
public:
  using World = std::unique_ptr<BlockDataWorldData>;

  explicit PlainWorldGenerator(const PlainWorldGeneratorSettings &settings
  ) noexcept;

  World operator()() noexcept;

private:
  const PlainWorldGeneratorSettings &_kSettings;

  World _world;
  core::Logger _logger;

  void _apply_structures() noexcept;
  void _override_blocks() noexcept;
};
}

#endif
