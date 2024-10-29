#include "game/world/structures/structure.hpp"
#include "game/world_generators/plain_world_generator.hpp"

namespace game
{
PlainWorldGenerator::PlainWorldGenerator(
  const PlainWorldGeneratorSettings &settings
) noexcept
  : _world(settings.w, settings.h(), settings.block_count()),
    _kSettings(settings), _logger("PlainWorldGenerator")
{
  _logger.info("Received these plain world generator settings:");
  _logger.info_fmt("    Width: %u", _kSettings.w);
  _logger.info_fmt("    Height: %u", _kSettings.h());

  _logger.info_fmt("    Block layers (%u):", _kSettings.layers.size());
  for (const auto &layer : _kSettings.layers)
  {
    _logger.info_fmt(
      "        x%u %s", layer.second, blocks::id_to_str(layer.first)
    );
  }

  if (!_kSettings.structures.empty())
  {
    _logger.info_fmt("    Structures (%u):", _kSettings.structures.size());
  }
  for (const WorldStructure &structure : _kSettings.structures)
  {
    _logger.info_fmt(
      "        %s at (%i; %i)",
      _kSettings.structureIdToStr(std::get<0>(structure)),
      std::get<1>(structure), std::get<2>(structure)
    );
  }
}

WorldData &PlainWorldGenerator::operator()() noexcept
{
  _logger.set_section("Generate");

  _logger.info_fmt("Generating %ux%u world", _kSettings.h(), _kSettings.w);

  unsigned short curLayer          = _kSettings.layers.size() - 1;
  unsigned short nextLayerRemained = _kSettings.layers[curLayer].second;
  for (unsigned short y = 0; y < _kSettings.h(); y++)
  {
    if (nextLayerRemained == 0)
    {
      curLayer--;
      nextLayerRemained = _kSettings.layers[curLayer].second;

      if (_kSettings.layers[curLayer].second == 0)
      {
        _logger.warn("Ignored layer with 0 height.");
        y--;
        continue;
      }
    }

    for (unsigned x = 0; x < _kSettings.w; x++)
    {
      _world[_blocksGenerated++] = {
        _kSettings.layers[curLayer].first, (int)x, y
      };
    }

    nextLayerRemained--;
  }

  if (!_kSettings.structures.empty())
  {
    _logger.info("Generating structures");
    for (const WorldStructure &structure : _kSettings.structures)
    {
      _logger.info_fmt(
        "Generating %s at (%i; %i)",
        _kSettings.structureIdToStr(std::get<0>(structure)),
        std::get<1>(structure), std::get<2>(structure)
      );

      _generate_structure(structure);
    }

    _logger.info("Structures generated.");
  }

  _logger.info("Plain world generated.");
  return _world;
}

void PlainWorldGenerator::_generate_structure(
  const WorldStructure &worldStructure
) noexcept
{
  const auto kStructure =
    _kSettings.structureFactory(std::get<0>(worldStructure));
  const auto kStructureData = kStructure->data();

  for (unsigned short y = 0; y < kStructure->h(); y++)
  {
    for (unsigned short x = 0; x < kStructure->w(); x++)
    {
      const BlockData &kBlock = kStructureData[y * kStructure->w() + x];
      if (!kBlock.enabled)
      {
        continue;
      }

      const int kX = std::get<1>(worldStructure) + kBlock.x,
                kY = std::get<2>(worldStructure) + kBlock.y;

      const auto kGeneratedBlock = _generatedStructureBlocks.find({kX, kY});
      if (kGeneratedBlock != _generatedStructureBlocks.cend())
      {
        _world[kGeneratedBlock->second] = {kBlock.id, kX, kY}; // Replace block.
        continue;
      }

      if (kX >= 0 && kX < (int)_kSettings.w && kY >= 0 && kY < _kSettings.h())
      {
        const unsigned long long kIdx = kY * _kSettings.w + kX;
        _generatedStructureBlocks.insert({{kX, kY}, kIdx});
        _world[kIdx] = {kBlock.id, kX, kY};
      }
      else
      {
        _generatedStructureBlocks.insert({{kX, kY}, _blocksGenerated});
        _world[_blocksGenerated++] = {kBlock.id, kX, kY};
      }
    }
  }
}
}
