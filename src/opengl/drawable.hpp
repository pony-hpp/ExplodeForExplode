#ifndef EFE_DRAWABLE_HPP
#define EFE_DRAWABLE_HPP

#include "core/types.hpp"

namespace gl
{
class Drawer;

class IDrawable
{
public:
  virtual int x() const noexcept { return 0; }
  virtual int y() const noexcept { return 0; }
  virtual uint w() const noexcept { return 0; }
  virtual uint h() const noexcept { return 0; }
  virtual bool always_draw() const noexcept { return false; }
};

// A drawable that draws OpenGL primitives directly.
class ISingleDrawable : public IDrawable
{
public:
  virtual void draw() const noexcept = 0;
};

// A drawable that draws other drawables through the drawer. The drawer checks
// whether the drawable is visible in the viewport, so you don't have to worry
// about it.
class ICompositeDrawable : public IDrawable
{
public:
  virtual void draw(Drawer &drawer) const noexcept = 0;
};
}

#endif
