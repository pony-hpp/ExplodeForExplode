#ifndef EFE_PNG_HPP
#define EFE_PNG_HPP

#include "core/logger.hpp"
#include "core/types.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace core
{
struct Png
{
  using Data = std::shared_ptr<ubyte[]>;

  inline explicit Png(ushort w, ushort h, Data data) noexcept
    : w(w), h(h), data(data)
  {}

  const ushort w, h;
  const Data data;
};

class PngDecoder final
{
public:
  inline PngDecoder() noexcept : _logger("PngDecoder") {}

  Png &operator()(const char *pngPath);

private:
  std::unordered_map<const char *, Png> _cachedPngs;
  std::vector<const char *> _fopenFailedPngPaths, _corruptedPngPaths;
  Logger _logger;
};

struct CorruptedPngException
{};
}

#endif
