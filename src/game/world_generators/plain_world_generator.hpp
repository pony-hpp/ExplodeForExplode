#ifndef _PLAIN_WORLD_GENERATOR_HPP_
#define _PLAIN_WORLD_GENERATOR_HPP_

#include "core/logger.hpp"
#include "game/world/structures/structure.hpp"
#include "game/world/world_data.hpp"

#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace game
{
struct WorldStructure
{
  unsigned char id;
  int x;
  int y;
};

class PlainWorldGeneratorSettings final
{
public:
  using BlockLayers = std::vector<std::pair<blocks::BlockId, unsigned short>>;
  using Structures  = std::vector<WorldStructure>;
  using StructureFactory = std::unique_ptr<Structure> (*)(unsigned char);

  unsigned w;
  BlockLayers layers;
  Structures structures;

  StructureFactory structureFactory = Structure::from_id;

  unsigned short h() const noexcept;

  // Don't generate structure data here, because they may depend on randomness.
  unsigned short
  extra_blocks(const std::vector<std::unique_ptr<BlockData[]>> &structures
  ) const noexcept;

private:
  mutable unsigned short _cachedH = 0;
};

class PlainWorldGenerator final
{
public:
  explicit PlainWorldGenerator(PlainWorldGeneratorSettings &settings) noexcept;

  std::unique_ptr<WorldData> operator()() noexcept;

private:
  const PlainWorldGeneratorSettings &_kSettings;
  std::unique_ptr<WorldData> _world;
  unsigned long long _blocksGenerated = 0;
  std::map<std::pair<int, int>, unsigned long long>
    _generatedStructureBlockIndexes;
  std::vector<std::unique_ptr<BlockData[]>> _generatedStructures;
  core::Logger _logger;

  void _generate_structure(unsigned short idx) noexcept;
};
}

#endif
