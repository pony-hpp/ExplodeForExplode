#include "game/world/world.hpp"

namespace game
{
World::World(const WorldData &worldData) noexcept
  : _w(worldData.w), _h(worldData.h), _kBlockCount(worldData.kBlockCount),
    _logger("World")
{
  _logger.set_section("Create");

  _logger.debug_fmt(
    "Converting %ux%u (%llu blocks, %u extra blocks) world data into real "
    "world",
    worldData.h, worldData.w, worldData.kBlockCount,
    worldData.kBlockCount - worldData.w * worldData.h
  );

  _data = std::make_unique<std::unique_ptr<Block>[]>(worldData.kBlockCount);
  for (unsigned long long i = 0; i < worldData.kBlockCount; i++)
  {
    _data[i] = Block::from_data(worldData[i]);
  }

  _logger.debug("Converted.");
}

void World::draw(const core::Renderer &renderer) const noexcept
{
  for (unsigned long long i = 0; i < _kBlockCount; i++)
  {
    _data[i]->draw(renderer);
  }
}

void World::load_textures(core::PngDecoder &pngDecoder) noexcept
{
  _logger.set_section("LoadTextures");

  _logger.info("Loading textures for world blocks");
  for (unsigned short y = 0; y < _h; y++)
  {
    _logger.progress_fmt(
      "Loading textures for row %u/%u (%.1f%%)", y + 1, _h,
      (y + 1) / (float)_h * 100
    );

    for (unsigned x = 0; x < _w; x++)
    {
      _data[y * _w + x]->load_texture(pngDecoder);
    }
  }

  if (_kBlockCount > _w * _h)
  {
    _logger.progress("Loading textures for extra blocks");
    for (unsigned long long i = _w * _h; i < _kBlockCount; i++)
    {
      _data[i]->load_texture(pngDecoder);
    }
  }

  _logger.info("Textures successfully loaded.");
}
}
