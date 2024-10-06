#include "core/window.hpp"
#include "game/blocks/block.hpp"
#include "opengl/math/matrix.hpp"
#include "opengl/shading/shader_program.hpp"

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

  game::Block blocks[21];
  for (unsigned char i = 0; i < 10; i++) {
    blocks[i].set_pos(5 + i, 5 + i / 2);
  }
  for (unsigned char i = 10; i < 21; i++) {
    blocks[i].set_pos(5 + i, 5 + i / 2 - i + 10);
  }

  win.set_bg(165, 190, 251);
  while (!win.closed()) {
    win.poll_events();
    win.clear();

    for (const game::Block &block : blocks) {
      win.draw(block);
    }

    win.update();
  }

  return 0;
}
