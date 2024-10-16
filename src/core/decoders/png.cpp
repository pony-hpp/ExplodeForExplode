#include "core/decoders/png.hpp"
#include "core/exception.hpp"
#include "core/utils.hpp"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <png.h>

namespace core {
static Logger *_pLogger;
static std::vector<const char *> *_pCorruptedPngFilenames;
static png_struct **_pLibpng;
static png_info **_pInfo;
static FILE **_pFp;
const char *_pFilename;

static void _cleanup() noexcept {
  png_destroy_info_struct(*_pLibpng, _pInfo);
  png_destroy_read_struct(_pLibpng, nullptr, nullptr);
  fclose(*_pFp);
}

static void _libpng_err_handler(png_struct *, const char *msg) {
  _pLogger->error_fmt("PNG decoding error: %s.", msg);
  if (std::find(
        _pCorruptedPngFilenames->cbegin(), _pCorruptedPngFilenames->cend(),
        _pFilename
      ) == std::cend(*_pCorruptedPngFilenames)) {
    _pCorruptedPngFilenames->push_back(_pFilename);
  }
  _cleanup();
  throw CorruptedPngException();
}

template <class T>
static void _throw_if_not_empty(
  const std::vector<const char *> &filenames, const char *filename
) {
  if (std::find(filenames.cbegin(), filenames.cend(), filename) !=
      std::cend(filenames)) {
    throw T();
  }
}

Png::Png(
  unsigned short w, unsigned short h, std::unique_ptr<unsigned char[]> data
) noexcept
  : data(std::move(data)), w(w), h(h) {
}

Png::Png(const Png &rhs) noexcept
  : data(std::move(rhs.data)), w(rhs.w), h(rhs.h) {
}

PngDecoder::PngDecoder() noexcept : _logger("PngDecoder") {
}

Png &PngDecoder::operator()(const char *filename) {
  _throw_if_not_empty<InvalidFileExtensionException>(
    _invalidExtensionFilenames, filename
  );
  _throw_if_not_empty<FopenException>(_fopenFailedPngFiles, filename);
  _throw_if_not_empty<CorruptedPngException>(_corruptedPngFilenames, filename);

  const auto &kCachedPng = _cachedPngs.find(filename);
  if (kCachedPng != _cachedPngs.cend()) {
    return kCachedPng->second;
  }

  _logger.debug("Checking for correct PNG file extension");
  try {
    check_file_extension(filename, "png");
  } catch (const InvalidFileExtensionException &e) {
    _logger.error_fmt(
      "Invalid file extension: png required, but %s provided.",
      e.provided.c_str()
    );
    _invalidExtensionFilenames.push_back(filename);
    throw core::InvalidFileExtensionException();
  }

  _logger.info_fmt("Decoding image \"%s\"", filename);

  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    _logger.error_fmt("Failed to open \"%s\": %s.", filename, strerror(errno));
    _fopenFailedPngFiles.push_back(filename);
    throw FopenException();
  }

  _logger.debug("Checking for correct PNG signature");
  unsigned char sig[8];
  fread(sig, 1, 8, fp);
  if (!png_check_sig(sig, 8)) {
    fclose(fp);
    _corruptedPngFilenames.push_back(filename);
    _logger.error("PNG signature is incorrect; image is corrupted.");
    throw CorruptedPngException();
  }

  png_struct *libpng = png_create_read_struct(
    PNG_LIBPNG_VER_STRING, nullptr, _libpng_err_handler, _libpng_err_handler
  );
  png_set_sig_bytes(libpng, 8);
  png_info *info = png_create_info_struct(libpng);
  png_init_io(libpng, fp);

  _pLogger                = &_logger;
  _pCorruptedPngFilenames = &_corruptedPngFilenames;
  _pLibpng                = &libpng;
  _pInfo                  = &info;
  _pFp                    = &fp;
  _pFilename              = filename;

  png_read_png(libpng, info, PNG_TRANSFORM_IDENTITY, nullptr);

  unsigned w, h;
  png_get_IHDR(
    libpng, info, &w, &h, nullptr, nullptr, nullptr, nullptr, nullptr
  );
  _logger.debug_fmt("Image size is %ux%u.", w, h);

  unsigned char **data = png_get_rows(libpng, info);
  std::unique_ptr<unsigned char[]> res(new unsigned char[w * h * 4]);

  for (unsigned i = 0; i < h; i++) {
    for (unsigned j = 0; j < w; j++) {
      const unsigned kIter = i * w + j;

      res[kIter * 4]     = data[i][j * 3];
      res[kIter * 4 + 1] = data[i][j * 3 + 1];
      res[kIter * 4 + 2] = data[i][j * 3 + 2];
      res[kIter * 4 + 3] = 255;
    }
  }

  _logger.debug("Caching image");
  _cachedPngs.insert(
    {filename, Png((unsigned short)w, (unsigned short)h, std::move(res))}
  );

  _cleanup();

  _logger.info("Image successfully decoded.");
  return _cachedPngs.find(filename)->second;
}
}
