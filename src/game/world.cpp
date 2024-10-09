#include "game/world.hpp"

namespace game {
World::World(const WorldData &data) noexcept : _kW(data.w), _kH(data.h) {
  _data = new std::unique_ptr<Block> *[data.h];
  for (unsigned short i = 0; i < data.h; i++) {
    _data[i] = new std::unique_ptr<Block>[data.w];
    for (unsigned j = 0; j < data.w; j++) {
      _data[i][j] = Block::from_data(data[i][j]);
    }
  }
}

World::~World() noexcept {
  for (unsigned short i = 0; i < _kH; i++) {
    delete[] _data[i];
  }
  delete[] _data;
}

void World::draw(
  unsigned short winW, unsigned short winH, float viewPosX, float viewPosY
) const noexcept {
  for (unsigned short i = 0; i < _kH; i++) {
    for (unsigned j = 0; j < _kW; j++) {
      _data[i][j]->draw(winW, winH, viewPosX, viewPosY);
    }
  }
}

void World::load_textures(core::PngDecoder &pngDecoder) noexcept {
  for (unsigned short i = 0; i < _kH; i++) {
    for (unsigned j = 0; j < _kW; j++) {
      _data[i][j]->load_texture(pngDecoder);
    }
  }
}
}
