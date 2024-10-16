#include "game/world.hpp"

namespace game {
World::World(const WorldData &data) noexcept
  : _kW(data.w), _kH(data.h), _logger("World") {
  _logger.set_section("Create");

  _logger.debug_fmt(
    "Converting %ux%u world data into real world", data.h, data.w
  );

  _data = new std::unique_ptr<Block> *[data.h];
  for (unsigned short i = 0; i < data.h; i++) {
    _data[i] = new std::unique_ptr<Block>[data.w];
    for (unsigned j = 0; j < data.w; j++) {
      _data[i][j] = Block::from_data(data[i][j]);
    }
  }

  _logger.debug("Converted.");
}

World::~World() noexcept {
  for (unsigned short i = 0; i < _kH; i++) {
    delete[] _data[i];
  }
  delete[] _data;
}

void World::draw(const core::Renderer &renderer) const noexcept {
  for (unsigned short i = 0; i < _kH; i++) {
    for (unsigned j = 0; j < _kW; j++) {
      _data[i][j]->draw(renderer);
    }
  }
}

void World::load_textures(core::PngDecoder &pngDecoder) noexcept {
  _logger.set_section("LoadTextures");

  _logger.info("Loading textures for world blocks");
  for (unsigned short i = 0; i < _kH; i++) {
    for (unsigned j = 0; j < _kW; j++) {
      _data[i][j]->load_texture(pngDecoder);
    }
    _logger.progress_fmt(
      "Loaded textures for row %u/%u (%.1f%%).", i + 1, _kH,
      (i + 1) / (float)_kH * 100
    );
  }

  _logger.info("Textures successfully loaded.");
}
}
