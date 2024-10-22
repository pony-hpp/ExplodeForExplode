#include "core/decoders/png.hpp"
#include "core/exception.hpp"
#include "core/utils.hpp"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <png.h>

namespace core {
Png::Png(
  unsigned short w, unsigned short h, std::shared_ptr<unsigned char[]> data
) noexcept
  : data(data), w(w), h(h) {
}

static Logger *_pLogger;
static std::vector<const char *> *_pCorruptedDataPngPaths;
static png_struct **_pLibpng;
static png_info **_pInfo;
static FILE **_pFp;
static const char *_curPngPath;

static void _cleanup() noexcept {
  png_destroy_info_struct(*_pLibpng, _pInfo);
  png_destroy_read_struct(_pLibpng, nullptr, nullptr);
  fclose(*_pFp);
}

static void _libpng_err_handler(png_struct *, const char *msg) {
  _pLogger->error_fmt("PNG decoding error: %s.", msg);
  if (std::find(
        _pCorruptedDataPngPaths->cbegin(), _pCorruptedDataPngPaths->cend(),
        _curPngPath
      ) == _pCorruptedDataPngPaths->cend()) {
    _pCorruptedDataPngPaths->push_back(_curPngPath);
  }
  _cleanup();
  throw CorruptedPngException();
}

PngDecoder::PngDecoder() noexcept : _logger("PngDecoder") {
}

template <class T>
static void
_throw_if_not_empty(const std::vector<const char *> &paths, const char *path) {
  if (std::find(paths.cbegin(), paths.cend(), path) != paths.cend()) {
    throw T();
  }
}

Png &PngDecoder::operator()(const char *pngPath) {
  const auto kCachedPng = _cachedPngs.find(pngPath);
  if (kCachedPng != _cachedPngs.cend()) {
    return kCachedPng->second;
  }

  _throw_if_not_empty<InvalidFileExtensionException>(
    _invalidExtensionPngPaths, pngPath
  );
  _throw_if_not_empty<FopenException>(_fopenFailedPngFiles, pngPath);
  _throw_if_not_empty<CorruptedPngException>(_corruptedDataPngPaths, pngPath);

  _logger.debug("Checking for correct PNG file extension");
  try {
    check_file_extension(pngPath, "png");
  } catch (const InvalidFileExtensionException &e) {
    _logger.error_fmt(
      "Invalid file extension: png required, but %s provided.",
      e.provided.c_str()
    );
    _invalidExtensionPngPaths.push_back(pngPath);
    throw InvalidFileExtensionException();
  }

  _logger.info_fmt("Decoding image \"%s\"", pngPath);

  FILE *fp = fopen(pngPath, "rb");
  if (!fp) {
    _logger.error_fmt("Failed to open \"%s\": %s.", pngPath, strerror(errno));
    _fopenFailedPngFiles.push_back(pngPath);
    throw FopenException();
  }

  _logger.debug("Checking for correct PNG signature");
  unsigned char sig[8];
  fread(sig, 1, 8, fp);
  if (!png_check_sig(sig, 8)) {
    _logger.error("PNG signature is incorrect; image is corrupted.");
    _corruptedDataPngPaths.push_back(pngPath);
    fclose(fp);
    throw CorruptedPngException();
  }

  png_struct *libpng = png_create_read_struct(
    PNG_LIBPNG_VER_STRING, nullptr, _libpng_err_handler, _libpng_err_handler
  );
  png_set_sig_bytes(libpng, 8);
  png_info *info = png_create_info_struct(libpng);
  png_init_io(libpng, fp);

  _pLogger                = &_logger;
  _pCorruptedDataPngPaths = &_corruptedDataPngPaths;
  _pLibpng                = &libpng;
  _pInfo                  = &info;
  _pFp                    = &fp;
  _curPngPath             = pngPath;
  png_read_png(libpng, info, PNG_TRANSFORM_IDENTITY, nullptr);

  unsigned w, h;
  png_get_IHDR(
    libpng, info, &w, &h, nullptr, nullptr, nullptr, nullptr, nullptr
  );
  _logger.debug_fmt("Image size is %ux%u.", w, h);

  unsigned char **data = png_get_rows(libpng, info);
  unsigned char *res   = new unsigned char[w * h * 4];

  for (unsigned short y = 0; y < h; y++) {
    for (unsigned short x = 0; x < w; x++) {
      const unsigned kIter = y * w + x;

      res[kIter * 4]     = data[y][x * 3];
      res[kIter * 4 + 1] = data[y][x * 3 + 1];
      res[kIter * 4 + 2] = data[y][x * 3 + 2];
      res[kIter * 4 + 3] = 255;
    }
  }

  _logger.debug("Caching image");
  _cachedPngs.insert({pngPath, Png(w, h, std::shared_ptr<unsigned char[]>(res))}
  );

  _cleanup();

  _logger.info("Image successfully decoded.");
  return _cachedPngs.find(pngPath)->second;
}
}
