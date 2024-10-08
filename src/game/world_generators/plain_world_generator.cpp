#include "game/world_generators/plain_world_generator.hpp"

namespace game {
unsigned short PlainWorldGeneratorSettings::h() const noexcept {
  if (!_cachedH) {
    for (const BlocksLayer &layer : layers) {
      _cachedH += layer.h;
    }
  }
  return _cachedH;
}

PlainWorldGenerator::PlainWorldGenerator(
  const PlainWorldGeneratorSettings &settings
) noexcept
  : _world(settings.w, settings.h()), _kSettings(settings) {
}

WorldData &PlainWorldGenerator::operator()() noexcept {
  unsigned short curLayer          = _kSettings.layers.size() - 1;
  unsigned short nextLayerRemained = _kSettings.layers[curLayer].h;
  for (short i = 0; i < _kSettings.h(); i++) {
    if (nextLayerRemained == 0) {
      curLayer--;
      nextLayerRemained = _kSettings.layers[curLayer].h;
      if (_kSettings.layers[curLayer].h == 0) { // Ignore layers with 0 height
        i--;
        continue;
      }
    }

    for (unsigned j = 0; j < _kSettings.w; j++) {
      _world[i][j] = {_kSettings.layers[curLayer].block, (int)j, i};
    }

    nextLayerRemained--;
  }

  return _world;
}
}
