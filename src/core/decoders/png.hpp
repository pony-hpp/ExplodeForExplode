#ifndef _PNG_HPP_
#define _PNG_HPP_

#include "core/logger.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace core {
struct Png {
  Png(
    unsigned short w, unsigned short h, std::unique_ptr<unsigned char[]> data
  ) noexcept;
  Png(const Png &rhs) noexcept;

  const unsigned short w, h;

  // Allow data to be moved in the copy constructor through the "mutable"
  // keyword. This is done because std::unordered_map requires a copy
  // constructor to insert, but this struct is move-only due to optimization
  // reasons.
  mutable std::unique_ptr<unsigned char[]> data;
};

struct CorruptedPngException {};

class PngDecoder final {
public:
  PngDecoder() noexcept;

  Png &operator()(const char *filename);

private:
  std::unordered_map<const char *, Png> _cachedPngs;
  std::vector<const char *> _fopenFailedPngFiles, _invalidExtensionFilenames,
    _corruptedPngFilenames;
  Logger _logger;
};
}

#endif
