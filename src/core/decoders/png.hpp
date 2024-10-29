#ifndef _PNG_HPP_
#define _PNG_HPP_

#include "core/logger.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace core
{
struct Png
{
  explicit Png(
    unsigned short w, unsigned short h, std::shared_ptr<unsigned char[]> data
  ) noexcept;

  const unsigned short w, h;
  const std::shared_ptr<unsigned char[]> data;
};

class PngDecoder final
{
public:
  PngDecoder() noexcept;

  Png &operator()(const char *pngPath);

private:
  std::unordered_map<const char *, Png> _cachedPngs;
  std::vector<const char *> _fopenFailedPngFiles, _corruptedDataPngPaths;
  Logger _logger;
};

struct CorruptedPngException
{
};
}

#endif
