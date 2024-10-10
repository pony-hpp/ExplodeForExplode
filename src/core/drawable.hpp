#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

namespace core {
class Renderer;

class IDrawable {
public:
  virtual void draw(const Renderer &renderer) const noexcept = 0;
};
}

#endif
