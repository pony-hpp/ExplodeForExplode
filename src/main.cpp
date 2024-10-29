#include "core/exception.hpp"
#include "core/logger.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game/movement.hpp"
#include "game/world/world.hpp"
#include "game/world_generators/plain_world_generator.hpp"
#include "game/zooming.hpp"
#include "opengl/shading/shader_program.hpp"

#include <random>

using RandInt =
#if _WIN32
  // For some stupid reason, Windows forbids using char in the
  // std::uniform_int_distribution<>.
  short
#else
  char
#endif
  ;

static constexpr float _MV_SENSITIVITY = 1.5f;

int main()
{
  core::Logger logger("Game");
  logger.info_fmt("Welcome to Explode for Explode %s!", EFE_VERSION);

  core::Window win(1024, 768, "Explode for Explode");
  try
  {
    win.create();
  }
  catch (const core::WindowCreationException &e)
  {
    logger.critical_fmt("The window couldn't be created: %s.", e.msg.c_str());
    return 1;
  };

  core::PngDecoder pngDecoder;
  try
  {
    win.set_icon(pngDecoder("../assets/textures/ui/window_icon.png"));
  }
  catch (const core::FopenException &)
  {
    logger.error_fmt("Failed to open window icon image.");
  }
  catch (const core::CorruptedPngException &)
  {
    logger.error_fmt("Window icon is corrupted.");
  }

  gl::VertexShader vs;
  gl::FragmentShader fs;
  gl::ShaderProgram shaderProgram;
  try
  {
    vs.load("../assets/shaders/shader.vert");
    vs.compile();
    fs.load("../assets/shaders/shader.frag");
    fs.compile();
    shaderProgram.add(vs);
    shaderProgram.add(fs);
    shaderProgram.link();
  }
  catch (...)
  {
    logger.critical("The shaders couldn't be compiled.");
    return 1;
  }

  shaderProgram.use();

  core::Renderer renderer(win, shaderProgram);

  game::Movement mv(_MV_SENSITIVITY);
  game::Zooming zoom(0.15f, 0.5f, 5.5f);

  bool rightBtnHeld = false;
  win.on_mouse_click([&win, &rightBtnHeld, &mv](
                       core::mouse::Button btn, core::mouse::Action action
                     ) noexcept
  {
    if (btn == core::mouse::BUTTON_RIGHT)
    {
      win.toggle_cursor_visibility();
      rightBtnHeld = action == core::mouse::ACTION_PRESS;
      if (!rightBtnHeld)
      {
        mv.set_next_origin();
      }
    }
  });

  win.on_cursor_move([&renderer, &shaderProgram, &rightBtnHeld, &mv,
                      &zoom](long long x, long long y) noexcept
  {
    if (rightBtnHeld)
    {
      mv(x / zoom.get().scale, y / zoom.get().scale);

      renderer.view.set_offset(
        zoom.get().offsetX + mv.get().x * zoom.get().scale,
        zoom.get().offsetY + mv.get().y * zoom.get().scale
      );

      shaderProgram.set_view_matrix(renderer.view);
    }
  });

  win.on_scroll([&win, &renderer, &shaderProgram, &zoom, &mv](bool up) noexcept
  {
    zoom(up, win.w() / 2.0f, win.h() / 2.0f);

    renderer.view.set_scale(zoom.get().scale);
    renderer.view.set_offset(
      zoom.get().offsetX + mv.get().x * zoom.get().scale,
      zoom.get().offsetY + mv.get().y * zoom.get().scale
    );

    shaderProgram.set_view_matrix(renderer.view);
  });

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 200;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 34});
  worldSettings.layers.push_back({game::blocks::STONE_BLOCK, 65});

  shaderProgram.set_uniform("uWorldH", worldSettings.h() * game::Block::SIZE);

  // Set origin.
  mv(
    (worldSettings.w / 2.0f) * game::Block::SIZE / _MV_SENSITIVITY,
    -(worldSettings.h() * game::Block::SIZE - 15 * game::Block::SIZE) /
      _MV_SENSITIVITY
  );
  mv(0, 0); // Apply position.
  mv.set_next_origin();
  renderer.view.set_offset(mv.get().x, mv.get().y);
  shaderProgram.set_view_matrix(renderer.view);

  std::random_device rdev;
  std::mt19937_64 seed(rdev());
  logger.debug_fmt("Spawning trees and grass with seed %li", seed());

  for (unsigned i = 0; i < worldSettings.w; i++)
  {
    if (std::uniform_int_distribution<RandInt>(0, 15)(seed) == 0)
    {
      worldSettings.structures.push_back(
        {game::structures::TREE, i, worldSettings.h()}
      );
    }
    else if (std::uniform_int_distribution<RandInt>(0, 6)(seed) != 0)
    {
      worldSettings.structures.push_back(
        {game::structures::GRASS, i, worldSettings.h()}
      );
    }
  }

  game::PlainWorldGenerator worldGen(worldSettings);
  game::World world = worldGen();
  world.load_textures(pngDecoder);

  win.set_bg(165, 190, 251);
  while (!win.closed())
  {
    win.poll_events();
    win.clear();
    renderer.draw(world);
    win.update();
  }

  logger.info("Goodbye!");
  return 0;
}
