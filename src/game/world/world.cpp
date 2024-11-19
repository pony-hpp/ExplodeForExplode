#include "game/world/world.hpp"
#include "opengl/window/renderer.hpp"

namespace game
{
World::World(const BlockDataWorldData &blocksData) noexcept
  : data(blocksData.w(), blocksData.h(), blocksData.extra_data_size()),
    _logger("World")
{
  _logger.set_section("Create");

  _logger.debug_fmt(
    "Converting %ux%u (with %u extra blocks) world data", blocksData.h(),
    blocksData.w(), blocksData.extra_data_size()
  );

  for (ullong i = 0; i < blocksData.dimensions_size(); i++)
  {
    data.push(Block::from_data(*blocksData.at(i)));
  }

  for (ullong i = blocksData.dimensions_size(); i < blocksData.size(); i++)
  {
    if (blocksData.at(i))
    {
      // Use block position as element position
      data.push(
        Block::from_data(*blocksData.at(i)),
        {blocksData.at(i)->x, blocksData.at(i)->y}
      );
    }
  }

  _logger.debug_fmt("Converted %llu blocks.", blocksData.size());
}

void World::draw(gl::Drawer &drawer) const noexcept
{
  for (ullong i = 0; i < data.size(); i++)
  {
    if (*data.at(i))
    {
      drawer.draw(**data.at(i));
    }
  }
}

void World::load_textures(core::PngDecoder &pngDecoder) noexcept
{
  _logger.set_section("LoadTextures");

  _logger.info("Loading textures for world blocks");

  for (uint y = 0; y < data.h(); y++)
  {
    const float kProgress = (y + 1) / (float)data.h() * 100;

    _logger.progress_fmt(
      "Loading textures for row %u/%u (%.1f%%)", y + 1, data.h(), kProgress
    );

    for (uint x = 0; x < data.w(); x++)
    {
      (*data.at(x, y))->load_texture(pngDecoder);
    }
  }

  if (data.extra_data_size())
  {
    _logger.progress("Loading textures for extra blocks");

    for (ullong i = data.dimensions_size(); i < data.size(); i++)
    {
      (*data.at(i))->load_texture(pngDecoder);
    }
  }

  _logger.info("Textures loaded.");
}
}
