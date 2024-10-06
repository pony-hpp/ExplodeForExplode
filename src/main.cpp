#include "core/window.hpp"

int main() {
  core::Window win(1024, 768, "Explode for explode");

  win.set_bg(165, 190, 251);
  while (!win.closed()) {
    win.poll_events();
    win.clear();
    win.update();
  }

  return 0;
}
