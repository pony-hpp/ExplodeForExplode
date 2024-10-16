#include "core/logger.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game/movement.hpp"
#include "game/world.hpp"
#include "game/world_generators/plain_world_generator.hpp"
#include "game/zooming.hpp"
#include "opengl/shading/shader_program.hpp"

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
    vs.load("../assets/shaders/shader.vs");
    vs.compile();
    fs.load("../assets/shaders/shader.fs");
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
  game::MovementOffset mvOffset = {}, zoomOffset = {};

  bool rightBtnHeld = false;
  win.on_mouse_click(
    [&win, &rightBtnHeld](core::mouse::Button btn, core::mouse::Action action) {
    if (btn == core::mouse::BUTTON_RIGHT) {
      win.toggle_cursor_visibility();
      if (action == core::mouse::ACTION_PRESS) {
        rightBtnHeld = true;
      } else {
        rightBtnHeld = false;
      }
    }
  }
  );
  win.on_cursor_move([&renderer, &shaderProgram, &mv, &mvOffset, &zoomOffset,
                      &rightBtnHeld](long long x, long long y) {
    if (rightBtnHeld) {
      mv(x, y);

      mvOffset = mv.get();
      renderer.view.set_offset(
        zoomOffset.x + mv.get().x, zoomOffset.y + mv.get().y
      );

      shaderProgram.view(renderer.view);
    } else {
      mv.set_next_origin();
    }
  });

  win.on_scroll([&win, &renderer, &shaderProgram, &zoom, &zoomOffset,
                 &mvOffset](bool up) {
    zoom(-mvOffset.x + win.cursor_x(), -mvOffset.y + win.cursor_y(), up);

    zoomOffset = {zoom.get().offsetX, zoom.get().offsetY};
    renderer.view.set_scale(zoom.get().scale);
    renderer.view.set_offset(
      mvOffset.x + zoom.get().offsetX, mvOffset.y + zoom.get().offsetY
    );

    shaderProgram.view(renderer.view);
  });

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 200;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 34});
  worldSettings.layers.push_back({game::blocks::DEFAULT_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::STONE_BLOCK, 65});
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
