#ifndef _SIZEABLE_HPP_
#define _SIZEABLE_HPP_

namespace core {
template <typename SizeT>
class ISizeable {
public:
  virtual SizeT w() const noexcept = 0;
  virtual SizeT h() const noexcept = 0;
};
}

#endif
