#include "core/window.hpp"
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

  win.on_resize([&shaderProgram](unsigned short w, unsigned short h) {
    shaderProgram.projection(gl::math::projection_matrix(w, h));
  });

  game::PlainWorldGeneratorSettings worldSettings;
  worldSettings.w = 100;
  worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
  worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 64});
  worldSettings.layers.push_back({game::blocks::DEFAULT_BLOCK, 10});
  game::PlainWorldGenerator worldGen(std::move(worldSettings));
  game::World world = worldGen();

  win.set_bg(165, 190, 251);
  while (!win.closed()) {
    win.poll_events();
    win.clear();
    win.draw(world);
    win.update();
  }

  return 0;
}
