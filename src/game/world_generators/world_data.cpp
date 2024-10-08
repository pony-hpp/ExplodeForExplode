#include "game/world_generators/world_data.hpp"

namespace game {
WorldData::WorldData(unsigned w, unsigned short h) noexcept : w(w), h(h) {
  _data = new BlockData *[h];
  for (unsigned short i = 0; i < h; i++) {
    _data[i] = new BlockData[w];
  }
}

WorldData::~WorldData() noexcept {
  for (unsigned short i = 0; i < h; i++) {
    delete[] _data[i];
  }
  delete[] _data;
}

BlockData *WorldData::operator[](unsigned short y) const noexcept {
  return _data[y];
}
}
