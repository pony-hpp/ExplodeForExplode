#include "core/exception.hpp"
#include "core/logger.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game/dynamites/dynamite.hpp"
#include "game/movement.hpp"
#include "game/world/world.hpp"
#include "game/world_generators/plain_world_generator.hpp"
#include "game/zooming.hpp"
#include "opengl/shading/shader_program.hpp"

#include <chrono>
#include <memory>
#include <random>
#include <thread>
#include <vector>

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
    logger.error_fmt("Window icon image is corrupted.");
  }

  gl::ShaderProgram worldShaderProgram("World"),
    dynamiteShaderProgram("Dynamite");
  try
  {
    gl::VertexShader worldVs;
    worldVs.load("../assets/shaders/world_shader.vert");
    worldVs.compile();
    gl::FragmentShader worldFs;
    worldFs.load("../assets/shaders/world_shader.frag");
    worldFs.compile();
    worldShaderProgram.add(worldVs);
    worldShaderProgram.add(worldFs);
    worldShaderProgram.link();

    gl::VertexShader dynamiteVs;
    dynamiteVs.load("../assets/shaders/dynamite_shader.vert");
    dynamiteVs.compile();
    gl::FragmentShader dynamiteFs;
    dynamiteFs.load("../assets/shaders/dynamite_shader.frag");
    dynamiteFs.compile();
    dynamiteShaderProgram.add(dynamiteVs);
    dynamiteShaderProgram.add(dynamiteFs);
    dynamiteShaderProgram.link();
  }
  catch (...)
  {
    logger.critical("The shaders couldn't be compiled.");
    return 1;
  }

  core::Renderer renderer(win);
  renderer.add_shader_program(worldShaderProgram);
  renderer.add_shader_program(dynamiteShaderProgram);
  renderer.init();

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 200;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 34});
  worldSettings.layers.push_back({game::blocks::STONE_BLOCK, 65});

  worldShaderProgram.set_uniform(
    "uWorldH", worldSettings.h() * game::Block::SIZE
  );

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
  worldShaderProgram.use();
  game::World world = worldGen();
  world.load_textures(pngDecoder);

  game::Movement mv(_MV_SENSITIVITY);
  game::Zooming zoom(0.15f, 0.5f, 5.5f);

  std::vector<std::pair<std::unique_ptr<game::Dynamite>, bool>> dynamites;
  bool rightBtnHeld = false;
  win.on_mouse_click(
    [&](core::mouse::Button btn, core::mouse::Action action) noexcept
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
    else if (btn == core::mouse::BUTTON_LEFT &&
             action == core::mouse::ACTION_PRESS)
    {
      static unsigned explodedDynamites;

      std::thread([&win, &dynamites]() noexcept
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(750));

        if (!win.closed()) // Avoid UB when window is closed.
        {
          dynamites[explodedDynamites].second = true;
          explodedDynamites++;
        }
      }).detach();

      dynamites.push_back({std::make_unique<game::Dynamite>(world), false});

      dynamites.back().first->load_texture(pngDecoder);

      const float kX = zoom.get().offsetX / zoom.get().scale + mv.get().x +
                       win.cursor_x() / zoom.get().scale,
                  kY = zoom.get().offsetY / zoom.get().scale + mv.get().y +
                       win.cursor_y() / zoom.get().scale;
      dynamites.back().first->set_pos(kX, kY);

      logger.info_fmt("Spawned dynamite at (%f; %f).", kX, kY);
    }
  }
  );

  win.on_cursor_move([&](int x, int y) noexcept
  {
    if (rightBtnHeld)
    {
      mv(x / zoom.get().scale, y / zoom.get().scale);

      renderer.view.set_offset(
        zoom.get().offsetX + mv.get().x * zoom.get().scale,
        zoom.get().offsetY + mv.get().y * zoom.get().scale
      );
      renderer.update_view();
    }
  });

  win.on_scroll([&renderer, &zoom, &mv](bool up) noexcept
  {
    zoom(up, renderer.viewport_w() / 2.0f, renderer.viewport_h() / 2.0f);

    renderer.view.set_scale(zoom.get().scale);
    renderer.view.set_offset(
      zoom.get().offsetX + mv.get().x * zoom.get().scale,
      zoom.get().offsetY + mv.get().y * zoom.get().scale
    );
    renderer.update_view();
  });

  // Set origin.
  mv(
    (worldSettings.w / 2.0f) * game::Block::SIZE / _MV_SENSITIVITY -
      win.w() / 2.0f,
    -(worldSettings.h() * game::Block::SIZE - 15 * game::Block::SIZE) /
      _MV_SENSITIVITY
  );
  mv(0, 0); // Apply position.
  mv.set_next_origin();
  renderer.view.set_offset(mv.get().x, mv.get().y);
  renderer.update_view();

  win.set_bg(165, 190, 251);
  while (!win.closed())
  {
    win.poll_events();
    win.clear();

    for (auto &dynamite : dynamites)
    {
      if (dynamite.first && dynamite.second)
      {
        dynamite.first->explode();
        dynamite.first.reset();
      }
    }

    worldShaderProgram.use();
    renderer.draw(world);

    dynamiteShaderProgram.use();
    for (const auto &dynamite : dynamites)
    {
      if (dynamite.first)
      {
        renderer.draw(*dynamite.first);
      }
    }

    win.update();
  }

  logger.info("Goodbye!");
  return 0;
}
