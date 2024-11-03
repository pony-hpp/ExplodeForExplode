#include "game/world_generators/plain_world_generator.hpp"

namespace game
{
static void _log_settings(
  const PlainWorldGeneratorSettings &settings, core::Logger &logger
) noexcept
{
  logger.info("Received these plain world generator settings:");
  logger.info_fmt("    Width: %u", settings.w);
  logger.info_fmt("    Height: %u", settings.h());

  logger.info_fmt("    Block layers (%u):", settings.layers.size());
  for (const auto &layer : settings.layers)
  {
    logger.info_fmt(
      "        x%u %s", layer.second, blocks::id_to_str(layer.first)
    );
  }

  if (!settings.structures.empty())
  {
    logger.info_fmt("    Structures (%u):", settings.structures.size());
  }
  for (const WorldStructure &structure : settings.structures)
  {
    logger.info_fmt(
      "        %s at (%i; %i)", settings.structureFactory(structure.id)->name(),
      structure.x, structure.y
    );
  }
}

PlainWorldGenerator::PlainWorldGenerator(
  const PlainWorldGeneratorSettings &settings
) noexcept
  : _kSettings(settings),
    _world(settings.w, settings.h(), settings.block_count()),
    _logger("PlainWorldGenerator")
{
  _log_settings(settings, _logger);
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
        _kSettings.structureFactory(structure.id)->name(), structure.x,
        structure.y
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
  const auto kStructure     = _kSettings.structureFactory(worldStructure.id);
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

      const int kAbsX = worldStructure.x + kBlock.x,
                kAbsY = worldStructure.y + kBlock.y;

      const auto kGeneratedBlock =
        _generatedStructureBlocksIndexes.find({kAbsX, kAbsY});
      if (kGeneratedBlock != _generatedStructureBlocksIndexes.cend())
      {
        // Replace block.
        _world[kGeneratedBlock->second] = {kBlock.id, kAbsX, kAbsY};
        continue;
      }

      if (kAbsX >= 0 && kAbsX < (int)_kSettings.w && kAbsY >= 0 &&
          kAbsY < _kSettings.h())
      {
        // Replace block inside world.
        const unsigned long long kIdx = kAbsY * _kSettings.w + kAbsX;
        _generatedStructureBlocksIndexes.insert({{kAbsX, kAbsY}, kIdx});
        _world[kIdx] = {kBlock.id, kAbsX, kAbsY};
      }
      else
      {
        // Generate extra block.
        _generatedStructureBlocksIndexes.insert(
          {{kAbsX, kAbsY}, _blocksGenerated}
        );
        _world[_blocksGenerated++] = {kBlock.id, kAbsX, kAbsY};
      }
    }
  }
}
}
