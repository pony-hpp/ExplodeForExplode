#include "core/decoders/png.hpp"

#include <gtest/gtest.h>

using namespace core;

#define RGB(r, g, b) r, g, b, 255

#define RED         RGB(255, 0, 0)
#define GREEN       RGB(0, 255, 0)
#define BLUE        RGB(0, 0, 255)
#define YELLOW      RGB(255, 255, 0)
#define MAGENTA     RGB(255, 0, 255)
#define CYAN        RGB(0, 255, 255)
#define WHITE       RGB(255, 255, 255)
#define GRAY        RGB(127, 127, 127)
#define BLACK       RGB(0, 0, 0)
#define TRANSPARENT 0, 0, 0, 0

static void _assert_png(
  const Png &png, ushort expectedW, ushort expectedH, const ubyte *expectedData
) noexcept
{
  ASSERT_EQ(png.w, expectedW);
  ASSERT_EQ(png.h, expectedH);

  for (uint i = 0; i < png.w * png.h * 4; i++)
  {
    ASSERT_EQ(png.data[i], expectedData[i]);
  }
}

static PngDecoder decode;

TEST(PngDecoder, Decode_4x4_RgbTest)
{
  constexpr ubyte EXPECTED_DATA[] = {
    // clang-format off
    YELLOW, MAGENTA, CYAN,  RED,
    BLUE,   BLACK,   WHITE, GREEN,
    GREEN,  GRAY,    BLACK, BLUE,
    RED,    MAGENTA, CYAN,  YELLOW
    // clang-format on
  };

  _assert_png(decode("../tests/assets/rgb_4x4.png"), 4, 4, EXPECTED_DATA);
}

TEST(PngDecoder, Decode_7x3_RgbTest)
{
  constexpr ubyte EXPECTED_DATA[] = {
    // clang-format off
    RED,   GREEN, BLUE,  MAGENTA, YELLOW, CYAN,  WHITE,
    BLACK, BLACK, BLACK, BLACK,   BLACK,  BLACK, GRAY,
    WHITE, WHITE, WHITE, WHITE,   WHITE,  WHITE, BLACK
    // clang-format on
  };

  _assert_png(decode("../tests/assets/rgb_7x3.png"), 7, 3, EXPECTED_DATA);
}

TEST(PngDecoder, Decode_3x7_RgbTest)
{
  constexpr ubyte EXPECTED_DATA[] = {
    // clang-format off
    RED,     GREEN,   BLUE,
    YELLOW,  YELLOW,  YELLOW,
    MAGENTA, MAGENTA, MAGENTA,
    CYAN,    CYAN,    CYAN,
    RED,     GREEN,   BLUE,
    RED,     GREEN,   GRAY,
    RED,     WHITE,   BLACK
    // clang-format on
  };

  _assert_png(decode("../tests/assets/rgb_3x7.png"), 3, 7, EXPECTED_DATA);
}

TEST(PngDecoder, DecodeRgbaTest)
{
  constexpr ubyte EXPECTED_DATA[] = {
    // clang-format off
    RED,         BLACK,       TRANSPARENT, TRANSPARENT,
    BLACK,       GREEN,       BLACK,       TRANSPARENT,
    TRANSPARENT, BLACK,       BLUE,        BLACK,
    TRANSPARENT, TRANSPARENT, BLACK,       WHITE
    // clang-format on
  };

  _assert_png(decode("../tests/assets/rgba.png"), 4, 4, EXPECTED_DATA);
}

TEST(PngDecoder, CorruptedPngCheckTest)
{
  ASSERT_THROW(decode("../tests/assets/corrupted.png"), CorruptedPngException);
}
