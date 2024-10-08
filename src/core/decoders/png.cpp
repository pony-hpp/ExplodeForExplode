#include "core/decoders/png.hpp"

#include <cstdio>
#include <png.h>

namespace core {
Png::Png(
  unsigned short w, unsigned short h, std::unique_ptr<unsigned char[]> data
) noexcept
  : data(std::move(data)), w(w), h(h) {
}

Png::Png(const Png &rhs) noexcept
  : data(std::move(rhs.data)), w(rhs.w), h(rhs.h) {
}

Png &PngDecoder::operator()(const char *filename) noexcept {
  const auto &kCachedPng = _cachedPngs.find(filename);
  if (kCachedPng != _cachedPngs.end()) {
    return kCachedPng->second;
  }

  FILE *fp = fopen(filename, "rb");
  png_struct *libpng =
    png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  png_info *info = png_create_info_struct(libpng);
  png_init_io(libpng, fp);
  png_read_png(libpng, info, PNG_TRANSFORM_IDENTITY, nullptr);

  unsigned w, h;
  png_get_IHDR(
    libpng, info, &w, &h, nullptr, nullptr, nullptr, nullptr, nullptr
  );
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

  Png png((unsigned short)w, (unsigned short)h, std::move(res));
  _cachedPngs.insert({filename, png});

  png_destroy_info_struct(libpng, &info);
  png_destroy_read_struct(&libpng, nullptr, nullptr);
  fclose(fp);
  return _cachedPngs.find(filename)->second;
}
}
