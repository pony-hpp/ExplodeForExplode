#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game/movement.hpp"
#include "game/world.hpp"
#include "game/world_generators/plain_world_generator.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader_program.hpp"

int main() {
  core::Window win(1024, 768, "Explode for Explode");

  gl::VertexShader vs;
  vs.load("../assets/shaders/shader.vs");
  vs.compile();
  gl::FragmentShader fs;
  fs.load("../assets/shaders/shader.fs");
  fs.compile();
  gl::ShaderProgram shaderProgram;
  shaderProgram.add(vs);
  shaderProgram.add(fs);
  shaderProgram.link();
  shaderProgram.use();

  core::Renderer renderer(win, shaderProgram);

  game::Movement mv(1.5f);
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
  win.on_cursor_move([&renderer, &shaderProgram, &mv,
                      &rightBtnHeld](long long x, long long y) {
    if (rightBtnHeld) {
      const game::MovementOffset &kOffset = mv(x, y);
      renderer.view.set_offset(kOffset.x, kOffset.y);
      shaderProgram.view(renderer.view);
    } else {
      mv.set_next_origin();
    }
  });

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 200;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 34});
  worldSettings.layers.push_back({game::blocks::STONE_BLOCK, 65});
  game::PlainWorldGenerator worldGen(std::move(worldSettings));

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

  return 0;
}
