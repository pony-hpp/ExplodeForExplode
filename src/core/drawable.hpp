#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

namespace core {
class IDrawable {
public:
  virtual void draw() const noexcept = 0;
};
}

#endif
