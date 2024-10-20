#include "game/world.hpp"

namespace game {
World::World(const WorldData &worldData) noexcept
  : _kW(worldData.w), _kH(worldData.h), _kBlockCount(worldData.kBlockCount),
    _logger("World") {
  _logger.set_section("Create");

  _logger.debug_fmt(
    "Converting %ux%u (%llu blocks, %u extra blocks) world data into real "
    "world",
    worldData.h, worldData.w, worldData.kBlockCount,
    worldData.kBlockCount - worldData.w * worldData.h
  );

  _data = std::make_unique<std::unique_ptr<Block>[]>(worldData.kBlockCount);
  for (unsigned i = 0; i < worldData.kBlockCount; i++) {
    _data[i] = Block::from_data(worldData[i]);
  }

  _logger.debug("Converted.");
}

void World::draw(const core::Renderer &renderer) const noexcept {
  for (unsigned short i = 0; i < _kBlockCount; i++) {
    _data[i]->draw(renderer);
  }
}

void World::load_textures(core::PngDecoder &pngDecoder) noexcept {
  _logger.set_section("LoadTextures");

  _logger.info("Loading textures for world blocks");
  for (unsigned short i = 0; i < _kH; i++) {
    _logger.progress_fmt(
      "Loading textures for row %u/%u (%.1f%%)", i + 1, _kH,
      (i + 1) / (float)_kH * 100
    );

    for (unsigned j = 0; j < _kW; j++) {
      _data[i * _kW + j]->load_texture(pngDecoder);
    }
  }

  if (_kBlockCount > _kW * _kH) {
    _logger.progress("Loading textures for extra blocks");
    for (unsigned long long i = _kW * _kH; i < _kBlockCount; i++) {
      _data[i]->load_texture(pngDecoder);
    }
  }

  _logger.info("Textures successfully loaded.");
}
}
