#include "game/world/world.hpp"
#include "opengl/window/renderer.hpp"

namespace game
{
World::World(BlockDataWorldData &&blocksDataParam) noexcept
  : blocks(
      blocksDataParam.w(), blocksDataParam.h(),
      blocksDataParam.extra_data_size()
    ),
    blocksData(std::move(blocksDataParam)), _logger("World")
{
  _logger.set_section("Create");

  _logger.debug_fmt(
    "Converting %ux%u (with %u extra blocks) world data", blocks.h(),
    blocks.w(), blocks.extra_data_size()
  );

  for (ullong i = 0; i < blocks.dimensions_size(); i++)
  {
    blocks.push(Block::from_data(*blocksData.at(i)));
  }

  for (ullong i = blocks.dimensions_size(); i < blocks.size(); i++)
  {
    if (blocksData.at(i))
    {
      // Use block position as element position
      blocks.push(
        Block::from_data(*blocksData.at(i)),
        {blocksData.at(i)->x, blocksData.at(i)->y}
      );
    }
  }

  _logger.debug_fmt("Converted %llu blocks.", blocks.size());
}

void World::draw(gl::Drawer &drawer) const noexcept
{
  for (ullong i = 0; i < blocks.size(); i++)
  {
    if (*blocks.at(i))
    {
      drawer.draw(**blocks.at(i));
    }
  }
}

void World::load_textures(core::PngDecoder &pngDecoder) noexcept
{
  _logger.set_section("LoadTextures");

  _logger.info("Loading textures for world blocks");

  for (uint y = 0; y < blocks.h(); y++)
  {
    const float kProgress = (y + 1) / (float)blocks.h() * 100;

    _logger.progress_fmt(
      "Loading textures for row %u/%u (%.1f%%)", y + 1, blocks.h(), kProgress
    );

    for (uint x = 0; x < blocks.w(); x++)
    {
      (*blocks.at(x, y))->load_texture(pngDecoder);
    }
  }

  if (blocks.extra_data_size())
  {
    _logger.progress("Loading textures for extra blocks");

    for (ullong i = blocks.dimensions_size(); i < blocks.size(); i++)
    {
      (*blocks.at(i))->load_texture(pngDecoder);
    }
  }

  _logger.info("Textures loaded.");
}
}
