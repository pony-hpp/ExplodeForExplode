#ifndef _PNG_HPP_
#define _PNG_HPP_

#include <memory>

namespace core {
struct Png {
  const std::unique_ptr<unsigned char> data;
  const unsigned short w, h;
};

extern Png decode_png(const char *filename) noexcept;
}

#endif
