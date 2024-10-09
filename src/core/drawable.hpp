#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

namespace core {
class IDrawable {
public:
  virtual void draw(
    unsigned short winW, unsigned short winH, float viewPosX, float viewPosY
  ) const noexcept = 0;
};
}

#endif
