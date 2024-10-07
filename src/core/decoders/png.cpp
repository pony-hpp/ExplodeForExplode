#include "core/decoders/png.hpp"

#include <cstdio>
#include <png.h>

core::Png core::decode_png(const char *filename) noexcept {
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
  std::unique_ptr<unsigned char> res(new unsigned char[w * h * 4]);

  for (unsigned i = 0; i < h; i++) {
    for (unsigned j = 0; j < w; j++) {
      const unsigned kIter     = i * w + j;
      res.get()[kIter * 4]     = data[i][j * 3];
      res.get()[kIter * 4 + 1] = data[i][j * 3 + 1];
      res.get()[kIter * 4 + 2] = data[i][j * 3 + 2];
      res.get()[kIter * 4 + 3] = 255;
    }
  }

  png_destroy_info_struct(libpng, &info);
  png_destroy_read_struct(&libpng, nullptr, nullptr);
  fclose(fp);
  return {std::move(res), (unsigned short)w, (unsigned short)h};
}
