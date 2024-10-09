#include "core/sizeable.hpp"
#include "game/movement.hpp"

#include <gtest/gtest.h>

class Viewport final : public core::ISizeable<unsigned char> {
public:
  Viewport(unsigned char w, unsigned char h) noexcept : _kW(w), _kH(h) {
  }

  unsigned char w() const noexcept override {
    return _kW;
  }

  unsigned char h() const noexcept override {
    return _kH;
  }

private:
  unsigned char _kW, _kH;
};

static const Viewport _kVp100x100 = Viewport(100, 100);
static const Viewport _kVp20x20   = Viewport(20, 20);

TEST(Movement, MoveXTest) {
  game::Movement mv(_kVp100x100, 1.0f);
  ASSERT_EQ(mv(50, 0).get_offset_x(), 0.0f); // Set origin.
  ASSERT_EQ(mv(25, 0).get_offset_x(), -0.25f);
  ASSERT_EQ(mv(75, 0).get_offset_x(), 0.25f);
  ASSERT_EQ(mv(100, 0).get_offset_x(), 0.5f);
  ASSERT_EQ(mv(0.0, 0).get_offset_x(), -0.5f);
  mv.set_next_origin();
  ASSERT_EQ(mv(0, 0).get_offset_x(), -0.5f);
  ASSERT_EQ(mv(10, 0).get_offset_x(), -0.4f);
  ASSERT_EQ(mv(25, 0).get_offset_x(), -0.25f);
  ASSERT_EQ(mv(50, 0).get_offset_x(), 0.0f);
  ASSERT_EQ(mv(0, 0).get_offset_x(), -0.5f);
}

TEST(Movement, MoveYTest) {
  game::Movement mv(_kVp100x100, 1.0f);
  ASSERT_EQ(mv(0, 0).get_offset_y(), 0.0f);
  // Note that the Y direction is inverted.
  ASSERT_EQ(mv(0, 33).get_offset_y(), -0.33f);
  ASSERT_EQ(mv(0, 75).get_offset_y(), -0.75f);
  ASSERT_EQ(mv(0, 100).get_offset_y(), -1.0f);
  ASSERT_EQ(mv(0, 0).get_offset_y(), 0.0f);
  mv.set_next_origin();
  ASSERT_EQ(mv(0, 100).get_offset_y(), 0.0f);
  ASSERT_EQ(mv(0, 75).get_offset_y(), 0.25f);
  ASSERT_EQ(mv(0, 0).get_offset_y(), 1.0f);
  ASSERT_EQ(mv(0, 50).get_offset_y(), 0.5f);
  ASSERT_EQ(mv(0, 33).get_offset_y(), 0.67f);
}

TEST(Movement, SensitivityTest) {
  game::Movement mv(_kVp20x20, 3.5f);
  mv(5, 10);
  const gl::math::ViewMatrix &kMat = mv(9, 4);
  ASSERT_NEAR(kMat.get_offset_x(), 0.2f * 3.5f, 0.0001f);
  ASSERT_NEAR(kMat.get_offset_y(), 0.3f * 3.5f, 0.0001f);
}
