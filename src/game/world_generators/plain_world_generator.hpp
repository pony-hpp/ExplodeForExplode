#ifndef _PLAIN_WORLD_GENERATOR_HPP_
#define _PLAIN_WORLD_GENERATOR_HPP_

#include "core/logger.hpp"
#include "game/world/blocks/block_id.hpp"
#include "game/world/structures/structure.hpp"
#include "game/world/structures/structure_id.hpp"
#include "game/world/world_data.hpp"

#include <map>
#include <utility>
#include <vector>

namespace game
{
using WorldStructure = std::tuple<unsigned char, int, int>;

class PlainWorldGeneratorSettings final
{
public:
  using BlockLayers = std::vector<std::pair<blocks::BlockId, unsigned short>>;
  using Structures  = std::vector<WorldStructure>;
  using StructureFactory = std::unique_ptr<Structure> (*)(unsigned char);
  using StructureIdToStr = const char *(*)(unsigned char);

  unsigned w;
  BlockLayers layers;
  Structures structures;

  StructureFactory structureFactory = Structure::from_id;
  StructureIdToStr structureIdToStr = structures::id_to_str;

  unsigned short h() const noexcept;
  unsigned long long block_count() const noexcept;

private:
  mutable unsigned short _cachedH = 0;
};

class PlainWorldGenerator final
{
public:
  explicit PlainWorldGenerator(const PlainWorldGeneratorSettings &settings
  ) noexcept;

  WorldData &operator()() noexcept;

private:
  const PlainWorldGeneratorSettings &_kSettings;
  WorldData _world;
  unsigned long long _blocksGenerated = 0;
  std::map<std::pair<int, int>, unsigned long long> _generatedStructureBlocks;
  core::Logger _logger;

  void _generate_structure(const WorldStructure &worldStructure) noexcept;
};
}

#endif
