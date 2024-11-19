#include "core/decoders/png.hpp"
#include "core/exception.hpp"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <png.h>

namespace core
{
static Logger *_pLogger;
static std::vector<const char *> *_pCorruptedPngPaths;
static png_struct **_pLibpng;
static png_info **_pInfo;
static FILE **_pFp;
static const char *_curPngPath;

static void _cleanup() noexcept
{
  fclose(*_pFp);
  png_destroy_read_struct(_pLibpng, nullptr, _pInfo);
}

static void _libpng_err_handler(png_struct *, const char *msg)
{
  _pLogger->error_fmt("PNG decoding error: %s.", msg);

  const auto kFoundCorruptedPng = std::find(
    _pCorruptedPngPaths->cbegin(), _pCorruptedPngPaths->cend(), _curPngPath
  );
  if (kFoundCorruptedPng == _pCorruptedPngPaths->cend())
  {
    _pCorruptedPngPaths->push_back(_curPngPath);
  }

  _cleanup();

  throw CorruptedPngException();
}

template <class E>
static void
_throw_if_found(const std::vector<const char *> &paths, const char *path)
{
  if (std::find(paths.cbegin(), paths.cend(), path) != paths.cend())
  {
    throw E();
  }
}

Png &PngDecoder::operator()(const char *pngPath)
{
  const auto kCachedPng = _cachedPngs.find(pngPath);
  if (kCachedPng != _cachedPngs.cend())
  {
    return kCachedPng->second;
  }

  _throw_if_found<FopenException>(_fopenFailedPngPaths, pngPath);
  _throw_if_found<CorruptedPngException>(_corruptedPngPaths, pngPath);

  FILE *fp = fopen(pngPath, "rb");
  if (!fp)
  {
    _logger.error_fmt("Failed to open \"%s\": %s.", pngPath, strerror(errno));
    _fopenFailedPngPaths.push_back(pngPath);
    throw FopenException();
  }

  _logger.info_fmt("Decoding image \"%s\"", pngPath);

  _logger.debug("Checking for correct PNG signature");

  ubyte sig[8];
  fread(sig, 1, 8, fp);
  if (!png_check_sig(sig, 8))
  {
    _logger.error("PNG signature is incorrect; image is corrupted.");
    _corruptedPngPaths.push_back(pngPath);
    fclose(fp);
    throw CorruptedPngException();
  }

  png_struct *libpng = png_create_read_struct(
    PNG_LIBPNG_VER_STRING, nullptr, _libpng_err_handler, _libpng_err_handler
  );
  png_info *info = png_create_info_struct(libpng);

  png_set_sig_bytes(libpng, 8);
  png_init_io(libpng, fp);

  _pLogger            = &_logger;
  _pCorruptedPngPaths = &_corruptedPngPaths;
  _pLibpng            = &libpng;
  _pInfo              = &info;
  _pFp                = &fp;
  _curPngPath         = pngPath;

  png_read_png(libpng, info, PNG_TRANSFORM_IDENTITY, nullptr);

  uint w, h;
  int pixFmt;
  png_get_IHDR(
    libpng, info, &w, &h, nullptr, &pixFmt, nullptr, nullptr, nullptr
  );
  const byte kChannels = pixFmt == PNG_COLOR_TYPE_RGB ? 3 : 4;

  _logger.debug_fmt("Image size: %ux%u.", w, h);
  _logger.debug_fmt("Image pixel format: %s.", kChannels == 3 ? "RGB" : "RGBA");

  ubyte **data = png_get_rows(libpng, info);
  ubyte *res   = new ubyte[w * h * 4];

  for (uint y = 0; y < h; y++)
  {
    for (uint x = 0; x < w; x++)
    {
      const uint kIter = y * w + x;

      res[kIter * 4]     = data[y][x * kChannels];
      res[kIter * 4 + 1] = data[y][x * kChannels + 1];
      res[kIter * 4 + 2] = data[y][x * kChannels + 2];

      if (kChannels == 4)
      {
        res[kIter * 4 + 3] = data[y][x * kChannels + 3];
      }
      else
      {
        res[kIter * 4 + 3] = 255;
      }
    }
  }

  _cachedPngs.insert({pngPath, Png(w, h, Png::Data(res))});

  _cleanup();

  _logger.info("Image successfully decoded.");
  return _cachedPngs.find(pngPath)->second;
}
}
