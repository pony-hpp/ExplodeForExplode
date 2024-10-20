#include "game/world_generators/plain_world_generator.hpp"
#include "game/world_generators/structures/structure.hpp"

#include <algorithm>

namespace game {
unsigned short PlainWorldGeneratorSettings::h() const noexcept {
  if (!_cachedH) {
    for (const std::pair<blocks::BlockId, unsigned short> &layer : layers) {
      _cachedH += layer.second;
    }
  }
  return _cachedH;
}

unsigned long long PlainWorldGeneratorSettings::block_count() const noexcept {
  unsigned long long res = w * h();
  if (structures.empty()) {
    return res;
  }

  std::vector<std::pair<int, int>> extraBlocks;
  for (const WorldStructure &worldStructure : structures) {
    const std::unique_ptr<Structure> kStructure =
      structureFactory(std::get<0>(worldStructure));
    const std::unique_ptr<BlockData[]> kStructureData = kStructure->data();

    for (unsigned short i = 0; i < kStructure->h(); i++) {
      for (unsigned short j = 0; j < kStructure->w(); j++) {
        const BlockData &kBlock = kStructureData[i * kStructure->w() + j];
        const int kX            = std::get<1>(worldStructure) + kBlock.x;
        const int kY            = std::get<2>(worldStructure) + kBlock.y;

        if (!kBlock.enabled ||
            std::find(
              std::cbegin(extraBlocks), std::cend(extraBlocks),
              std::pair<int, int>(kX, kY)
            ) != std::cend(extraBlocks) ||
            ((kY < h() && kY >= 0) && (kX < (int)w && kX >= 0))) {
          continue;
        }

        extraBlocks.push_back({kX, kY});
        res++;
      }
    }
  }

  return res;
}

PlainWorldGenerator::PlainWorldGenerator(
  const PlainWorldGeneratorSettings &settings
) noexcept
  : _world(settings.w, settings.h(), settings.block_count()),
    _kSettings(settings), _logger("PlainWorldGenerator") {
  _logger.info("Received these plain world generator settings:");
  _logger.info_fmt("    Width: %u", _kSettings.w);
  _logger.info_fmt("    Height: %u", _kSettings.h());

  _logger.info_fmt("    Block layers (%u):", _kSettings.layers.size());
  for (const std::pair<blocks::BlockId, unsigned short> &layer :
       _kSettings.layers) {
    _logger.info_fmt(
      "        x%u %s", layer.second, blocks::id_to_str(layer.first)
    );
  }

  if (!_kSettings.structures.empty()) {
    _logger.info_fmt("    Structures (%u):", _kSettings.structures.size());
  }
  for (const WorldStructure &structure : _kSettings.structures) {
    _logger.info_fmt(
      "        %s at (%i; %i)",
      _kSettings.structureIdToStr(std::get<0>(structure)),
      std::get<1>(structure), std::get<2>(structure)
    );
  }
}

WorldData &PlainWorldGenerator::operator()() noexcept {
  _logger.set_section("Generate");

  _logger.info_fmt("Generating %ux%u world", _kSettings.h(), _kSettings.w);

  unsigned short curLayer          = _kSettings.layers.size() - 1;
  unsigned short nextLayerRemained = _kSettings.layers[curLayer].second;
  for (short i = 0; i < _kSettings.h(); i++) {
    if (nextLayerRemained == 0) {
      curLayer--;
      nextLayerRemained = _kSettings.layers[curLayer].second;

      if (_kSettings.layers[curLayer].second == 0) {
        _logger.warn("Ignored layer with 0 height.");
        i--;
        continue;
      }
    }

    for (unsigned j = 0; j < _kSettings.w; j++) {
      _world[i * _kSettings.w + j] = {
        _kSettings.layers[curLayer].first, (int)j, i
      };
      _blocksGenerated++;
    }

    nextLayerRemained--;
  }

  if (!_kSettings.structures.empty()) {
    _logger.info("Generating structures");
    for (const WorldStructure &structure : _kSettings.structures) {
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
) noexcept {
  const std::unique_ptr<Structure> kStructure =
    _kSettings.structureFactory(std::get<0>(worldStructure));
  const std::unique_ptr<BlockData[]> kStructureData = kStructure->data();

  for (unsigned short i = 0; i < kStructure->h(); i++) {
    for (unsigned short j = 0; j < kStructure->w(); j++) {
      const BlockData &kBlock = kStructureData[i * kStructure->w() + j];
      const int kX            = std::get<1>(worldStructure) + kBlock.x,
                kY            = std::get<2>(worldStructure) + kBlock.y;

      if (!kBlock.enabled ||
          std::find(
            std::cbegin(_drawn), std::cend(_drawn), std::pair<int, int>(kX, kY)
          ) != std::cend(_drawn)) {
        continue;
      }

      const unsigned long long kIdx = kY * _kSettings.w + kX;
      _drawn.push_back({kX, kY});
      if (kIdx < _world.w * _world.h) {
        _world[kIdx] = {kBlock.id, kX, kY};
      } else {
        _world[_blocksGenerated++] = {kBlock.id, kX, kY};
      }
    }
  }
}
}
