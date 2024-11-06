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

PlainWorldGenerator::PlainWorldGenerator(PlainWorldGeneratorSettings &settings
) noexcept
  : _kSettings(settings), _logger("PlainWorldGenerator")
{
  for (const WorldStructure &worldStructure : _kSettings.structures)
  {
    _generatedStructures.push_back(
      _kSettings.structureFactory(worldStructure.id)->data()
    );
  }

  _world = std::make_unique<WorldData>(
    settings.w, settings.h(), settings.extra_blocks(_generatedStructures)
  );

  _log_settings(settings, _logger);
}

std::unique_ptr<WorldData> PlainWorldGenerator::operator()() noexcept
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
      _world->at(_blocksGenerated++
      ) = {_kSettings.layers[curLayer].first, (int)x, y};
    }

    nextLayerRemained--;
  }

  if (!_kSettings.structures.empty())
  {
    _logger.info("Generating structures");
    for (unsigned short i = 0; i < _kSettings.structures.size(); i++)
    {
      _logger.info_fmt(
        "Generating %s at (%i; %i)",
        _kSettings.structureFactory(_kSettings.structures[i].id)->name(),
        _kSettings.structures[i].x, _kSettings.structures[i].y
      );

      _generate_structure(i);
    }

    _logger.info("Structures generated.");
  }

  _logger.info("Plain world generated.");
  return std::move(_world);
}

void PlainWorldGenerator::_generate_structure(unsigned short idx) noexcept
{
  const auto kStructure =
    _kSettings.structureFactory(_kSettings.structures[idx].id);

  for (unsigned short y = 0; y < kStructure->h(); y++)
  {
    for (unsigned short x = 0; x < kStructure->w(); x++)
    {
      const BlockData &kBlock =
        _generatedStructures[idx][y * kStructure->w() + x];
      if (!kBlock.enabled)
      {
        continue;
      }

      const int kAbsX = _kSettings.structures[idx].x + kBlock.x,
                kAbsY = _kSettings.structures[idx].y + kBlock.y;

      const auto kGeneratedBlock =
        _generatedStructureBlockIndexes.find({kAbsX, kAbsY});
      if (kGeneratedBlock != _generatedStructureBlockIndexes.cend())
      {
        // Replace block.
        _world->at(kGeneratedBlock->second) = {kBlock.id, kAbsX, kAbsY};
        continue;
      }

      if (kAbsX >= 0 && kAbsX < (int)_kSettings.w && kAbsY >= 0 &&
          kAbsY < _kSettings.h())
      {
        // Replace block inside world.
        const unsigned long long kIdx = kAbsY * _kSettings.w + kAbsX;
        _generatedStructureBlockIndexes.insert({{kAbsX, kAbsY}, kIdx});
        _world->at(kIdx) = {kBlock.id, kAbsX, kAbsY};
      }
      else
      {
        // Generate extra block.
        _generatedStructureBlockIndexes.insert(
          {{kAbsX, kAbsY}, _blocksGenerated}
        );
        _world->at(_blocksGenerated++) = {kBlock.id, kAbsX, kAbsY};
      }
    }
  }
}
}
