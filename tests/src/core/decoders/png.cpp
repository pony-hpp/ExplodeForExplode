#include "core/decoders/png.hpp"

#include <gtest/gtest.h>

using namespace core;

// clang-format off
#define RED     255, 0,   0,   255
#define GREEN   0,   255, 0,   255
#define BLUE    0,   0,   255, 255
#define YELLOW  255, 255, 0,   255
#define MAGENTA 255, 0,   255, 255
#define CYAN    0,   255, 255, 255
#define WHITE   255, 255, 255, 255
#define GRAY    127, 127, 127, 255
#define BLACK   0,   0,   0,   255
// clang-format on

static void _assert_png(
  const Png &png, unsigned short expectedW, unsigned short expectedH,
  const unsigned char *expectedData
) noexcept {
  ASSERT_EQ(png.w, expectedW);
  ASSERT_EQ(png.h, expectedH);
  ASSERT_NE(png.data.get(), nullptr);
  for (unsigned i = 0; i < png.w * png.h * 4; i++) {
    ASSERT_EQ(png.data[i], expectedData[i]);
  }
}

static PngDecoder decode;

TEST(PngDecoder, Decode_4x4_RgbTest) {
  constexpr unsigned char EXPECTED_DATA[] = {
    // clang-format off
    YELLOW, MAGENTA, CYAN,  RED,
    BLUE,   BLACK,   WHITE, GREEN,
    GREEN,  GRAY,    BLACK, BLUE,
    RED,    MAGENTA, CYAN,  YELLOW
    // clang-format on
  };
  _assert_png(decode("../tests/assets/rgb_4x4.png"), 4, 4, EXPECTED_DATA);
}

TEST(PngDecoder, Decode_7x3_RgbTest) {
  constexpr unsigned char EXPECTED_DATA[] = {
    RED,   GREEN, BLUE,  MAGENTA, YELLOW, CYAN,  WHITE,
    BLACK, BLACK, BLACK, BLACK,   BLACK,  BLACK, GRAY,
    WHITE, WHITE, WHITE, WHITE,   WHITE,  WHITE, BLACK
  };
  _assert_png(decode("../tests/assets/rgb_7x3.png"), 7, 3, EXPECTED_DATA);
}

TEST(PngDecoder, Decode_3x7_RgbTest) {
  constexpr unsigned char EXPECTED_DATA[] = {
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

TEST(PngDecoder, CorruptedPngCheckTest) {
  ASSERT_THROW(decode("../tests/assets/corrupted.png"), CorruptedPngException);
}
