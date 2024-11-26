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

  _logger.debug_fmt("Loading textures");

  for (ullong i = 0; i < blocks.size(); i++)
  {
    blocks.at(i)->get()->load_texture(pngDecoder);
  }

  _logger.debug_fmt("Textures loaded.");
}
}
