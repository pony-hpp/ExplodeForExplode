#include "core/logger.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game/movement.hpp"
#include "game/world/world.hpp"
#include "game/world_generators/plain_world_generator.hpp"
#include "game/zooming.hpp"
#include "opengl/shading/shader_program.hpp"

#include <random>

int main() {
  core::Logger logger("Game");
  logger.info_fmt("Welcome to Explode for Explode %s!", EFE_VERSION);

  core::Window win(1024, 768, "Explode for Explode");
  try {
    win.create();
  } catch (const core::WindowCreationException &e) {
    logger.critical_fmt("The window couldn't be created: %s.", e.msg.c_str());
    return 1;
  };

  gl::VertexShader vs;
  gl::FragmentShader fs;
  gl::ShaderProgram shaderProgram;
  try {
    vs.load("../assets/shaders/shader.vert");
    vs.compile();
    fs.load("../assets/shaders/shader.frag");
    fs.compile();
    shaderProgram.add(vs);
    shaderProgram.add(fs);
    shaderProgram.link();
  } catch (...) {
    logger.critical("The shaders couldn't be compiled.");
    return 1;
  }
  shaderProgram.use();

  core::Renderer renderer(win, shaderProgram);

  game::Movement mv(1.5f);
  game::Zooming zoom(0.15f, 0.5f, 5.5f);

  bool rightBtnHeld = false;
  win.on_mouse_click([&win, &rightBtnHeld, &mv](
                       core::mouse::Button btn, core::mouse::Action action
                     ) {
    if (btn == core::mouse::BUTTON_RIGHT) {
      win.toggle_cursor_visibility();
      if (action == core::mouse::ACTION_PRESS) {
        rightBtnHeld = true;
      } else {
        rightBtnHeld = false;
        mv.set_next_origin();
      }
    }
  });

  win.on_cursor_move([&renderer, &shaderProgram, &mv, &zoom,
                      &rightBtnHeld](long long x, long long y) {
    if (rightBtnHeld) {
      mv(x, y);

      renderer.view.set_offset(
        zoom.get().offsetX + mv.get().x, zoom.get().offsetY + mv.get().y
      );

      shaderProgram.view_matrix(renderer.view);
    }
  });

  win.on_scroll([&win, &renderer, &shaderProgram, &zoom, &mv](bool up) {
    zoom(up, mv.get().x + win.cursor_x(), mv.get().y + win.cursor_y());

    renderer.view.set_scale(zoom.get().scale);
    renderer.view.set_offset(
      mv.get().x + zoom.get().offsetX, mv.get().y + zoom.get().offsetY
    );

    shaderProgram.view_matrix(renderer.view);
  });

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 200;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 34});
  worldSettings.layers.push_back({game::blocks::DEFAULT_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::STONE_BLOCK, 65});

  std::random_device rdev;
  std::mt19937_64 seed(rdev());
  logger.debug_fmt("Spawning trees with seed %li", seed());

  for (unsigned i = 0; i < worldSettings.w; i++) {
    if (std::uniform_int_distribution<char>(0, 15)(seed) == 0) {
      worldSettings.structures.push_back(
        {game::structures::TREE, i, worldSettings.h()}
      );
    }
  }

  game::PlainWorldGenerator worldGen(worldSettings);

  core::PngDecoder pngDecoder;
  game::World world = worldGen();
  world.load_textures(pngDecoder);

  win.set_bg(165, 190, 251);
  while (!win.closed()) {
    win.poll_events();
    win.clear();
    renderer.draw(world);
    win.update();
  }

  logger.info("Goodbye!");
  return 0;
}
