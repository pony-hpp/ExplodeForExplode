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

  shaderProgram.view(gl::math::DEFAULT_MATRIX);

  win.on_resize([&shaderProgram](unsigned short w, unsigned short h) {
    shaderProgram.projection(gl::math::projection_matrix(w, h));
  });

  game::Movement mv(win, 600.0f);
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
  win.on_cursor_move([&win, &shaderProgram, &mv,
                      &rightBtnHeld](long long x, long long y) {
    if (rightBtnHeld) {
      const gl::math::Matrix &kMat = mv(x, y);
      shaderProgram.view(kMat);
      win.set_view_pos(kMat[3], kMat[7]);
    } else {
      mv.set_next_origin();
    }
  });

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 144;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 199});
  worldSettings.layers.push_back({game::blocks::DEFAULT_BLOCK, 32});
  game::PlainWorldGenerator worldGen(std::move(worldSettings));

  core::PngDecoder pngDecoder;
  game::World world = worldGen();
  world.load_textures(pngDecoder);

  win.set_bg(165, 190, 251);
  while (!win.closed()) {
    win.poll_events();
    win.clear();
    win.draw(world);
    win.update();
  }

  return 0;
}
