#include "game/world_generators/plain_world_generator.hpp"

namespace game {
unsigned short PlainWorldGeneratorSettings::h() const noexcept {
  if (!_cachedH) {
    for (const std::pair<blocks::BlockId, unsigned short> &layer : layers) {
      _cachedH += layer.second;
    }
  }
  return _cachedH;
}

PlainWorldGenerator::PlainWorldGenerator(
  const PlainWorldGeneratorSettings &settings
) noexcept
  : _world(settings.w, settings.h()), _kSettings(settings),
    _logger("PlainWorldGenerator") {
  _logger.info("Received these plain world generator settings:");
  _logger.info_fmt("    Width: %u", _kSettings.w);
  _logger.info_fmt("    Height: %u", _kSettings.h());
  _logger.info("    Block layers:");
  for (const std::pair<blocks::BlockId, unsigned short> &layer :
       _kSettings.layers) {
    _logger.info_fmt(
      "        x%u %s", layer.second, blocks::id_to_str(layer.first)
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
      _world[i][j] = {_kSettings.layers[curLayer].first, (int)j, i};
    }

    nextLayerRemained--;
  }

  _logger.info("Plain world generated.");
  return _world;
}
}
