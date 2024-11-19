#include "game/world/generators/plain_world_generator.hpp"

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
  for (const auto &kLayer : settings.layers)
  {
    logger.info_fmt(
      "        x%u %s", kLayer.second, blocks::id_to_str(kLayer.first)
    );
  }

  if (!settings.structuresGenerator)
  {
    return;
  }

  if (settings.structuresGenerator->size())
  {
    logger.info_fmt(
      "    Structures (%u):", settings.structuresGenerator->size()
    );
  }
  for (const WorldStructure &kStructure :
       settings.structuresGenerator->structures())
  {
    const auto kGeneratedStructure =
      settings.structuresGenerator->factory(kStructure.id);

    logger.info_fmt(
      "        %s at (%i; %i)", kGeneratedStructure->name(), kStructure.x,
      kStructure.y
    );
  }
}

PlainWorldGenerator::PlainWorldGenerator(
  const PlainWorldGeneratorSettings &settings
) noexcept
  : _kSettings(settings), _logger("PlainWorldGenerator")
{
  _log_settings(settings, _logger);

  _world = std::make_unique<BlockDataWorldData>(
    _kSettings.w, _kSettings.h(), _kSettings.extra_blocks()
  );
}

PlainWorldGenerator::World PlainWorldGenerator::operator()() noexcept
{
  _logger.set_section("Generate");

  _logger.info("Generating world");

  if (_kSettings.layers.empty())
  {
    return std::move(_world);
  }

  size_t curLayer          = _kSettings.layers.size() - 1;
  ushort nextLayerRemained = _kSettings.layers[curLayer].second;
  for (ushort y = 0; y < _kSettings.h(); y++)
  {
    if (nextLayerRemained == 0)
    {
      curLayer--;
      nextLayerRemained = _kSettings.layers[curLayer].second;

      if (!_kSettings.layers[curLayer].second)
      {
        _logger.warn("Ignored layer with 0 height.");
        y--;
        continue;
      }
    }

    for (uint x = 0; x < _kSettings.w; x++)
    {
      _world->push({_kSettings.layers[curLayer].first, (int)x, y});
    }

    nextLayerRemained--;
  }

  if (_kSettings.structuresGenerator)
  {
    _logger.debug("Applying structures");
    _apply_structures();
  }

  if (!_kSettings.overridenBlocks.empty())
  {
    _logger.debug("Overriding blocks");
    _override_blocks();
  }

  _logger.info("Plain world generated.");
  return std::move(_world);
}

void PlainWorldGenerator::_apply_structures() noexcept
{
  for (size_t i = 0; i < _kSettings.structuresGenerator->size(); i++)
  {
    const auto &kData = _kSettings.structuresGenerator->structuresData()[i];

    for (ullong j = 0; j < kData.size(); j++)
    {
      const BlockData *kBlock = kData.at(j);
      if (!kBlock)
      {
        continue;
      }

      if ((kBlock->x >= 0 && kBlock->x < (int)_kSettings.w) &&
          (kBlock->y >= 0 && kBlock->y < _kSettings.h()))
      {
        // Replace a block inside the world
        *_world->at(kBlock->x, kBlock->y) = *kBlock;
      }
      else
      {
        // Generate an extra block
        _world->push(*kBlock, {kBlock->x, kBlock->y});
      }
    }
  }
}

void PlainWorldGenerator::_override_blocks() noexcept
{
  for (const BlockData &kBlock : _kSettings.overridenBlocks)
  {
    ullong idx;
    bool idxFound = false;
    try
    {
      idx      = _world->idx(kBlock.x, kBlock.y);
      idxFound = true;
    }
    catch (const utils::NonexistentExtraElementException &)
    {}

    if (idxFound && idx < _world->dimensions_size())
    {
      // Override a block inside the world
      *_world->at(kBlock.x, kBlock.y) = kBlock;
    }
    else
    {
      // Generate an extra block
      _world->push(kBlock, {kBlock.x, kBlock.y});
    }
  }
}
}
