#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

namespace gl
{
constexpr float TEX_COORDS[] = {
  // clang-format off
  0.0f, 1.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f,
  // clang-format on
};

constexpr unsigned char FALLBACK_TEX[] = {
  // clang-format off
  0,   0, 0,   255,  255, 0,  255, 255,
  255, 0, 255, 255,  0,   0,  0,   255
  // clang-format on
};
}

#endif
