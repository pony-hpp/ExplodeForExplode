#include "core/window.hpp"
#include "game/blocks/blocks.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader_program.hpp"

#include <memory>

int main() {
  core::Window win(1024, 768, "Explode for explode");

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

  std::unique_ptr<game::Block> blocks[21];
  for (unsigned char i = 0; i < 21; i++) {
    if (i % 3 == 0) {
      blocks[i] = std::make_unique<game::blocks::GrassBlock>();
    } else if (i % 2 == 0) {
      blocks[i] = std::make_unique<game::blocks::EarthBlock>();
    } else {
      blocks[i] = std::make_unique<game::blocks::DefaultBlock>();
    }
  }

  for (unsigned char i = 0; i < 10; i++) {
    blocks[i]->set_pos(5 + i, 5 + i / 2);
    blocks[i]->load_texture();
  }
  for (unsigned char i = 10; i < 21; i++) {
    blocks[i]->set_pos(5 + i, 5 + i / 2 - i + 10);
    blocks[i]->load_texture();
  }

  win.set_bg(165, 190, 251);
  while (!win.closed()) {
    win.poll_events();
    win.clear();

    for (const auto &block : blocks) {
      win.draw(*block);
    }

    win.update();
  }

  return 0;
}
