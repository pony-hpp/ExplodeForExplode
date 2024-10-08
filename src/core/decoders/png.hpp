#ifndef _PNG_HPP_
#define _PNG_HPP_

#include <memory>
#include <unordered_map>

namespace core {
struct Png {
  Png(
    unsigned short w, unsigned short h, std::unique_ptr<unsigned char[]> data
  ) noexcept;
  Png(const Png &rhs) noexcept;

  // Allow data to be moved in the copy constructor through the "mutable"
  // keyword. This is done because std::unordered_map requires a copy
  // constructor to insert, but this struct is move-only due to optimization
  // reasons.
  mutable std::unique_ptr<unsigned char[]> data;
  const unsigned short w, h;
};

class PngDecoder final {
public:
  Png &operator()(const char *filename) noexcept;

private:
  std::unordered_map<const char *, Png> _cachedPngs;
};
}

#endif
