#include "game/movement.hpp"

#include <gtest/gtest.h>

TEST(Movement, MoveXTest) {
  game::Movement mv(1.0f);
  ASSERT_EQ(mv(50, 0).get_offset_x(), 0.0f); // Set origin.
  ASSERT_EQ(mv(25, 0).get_offset_x(), -25.0f);
  ASSERT_EQ(mv(75, 0).get_offset_x(), 25.0f);
  ASSERT_EQ(mv(100, 0).get_offset_x(), 50.0f);
  ASSERT_EQ(mv(0.0, 0).get_offset_x(), -50.0f);
  mv.set_next_origin();
  ASSERT_EQ(mv(0, 0).get_offset_x(), -50.0f);
  ASSERT_EQ(mv(10, 0).get_offset_x(), -40.0f);
  ASSERT_EQ(mv(25, 0).get_offset_x(), -25.0f);
  ASSERT_EQ(mv(50, 0).get_offset_x(), 0.0f);
  ASSERT_EQ(mv(0, 0).get_offset_x(), -50.0f);
}

TEST(Movement, MoveYTest) {
  game::Movement mv(1.0f);
  ASSERT_EQ(mv(0, 0).get_offset_y(), 0.0f);
  // Note that the Y direction is inverted.
  ASSERT_EQ(mv(0, 33).get_offset_y(), -33.0f);
  ASSERT_EQ(mv(0, 75).get_offset_y(), -75.0f);
  ASSERT_EQ(mv(0, 100).get_offset_y(), -100.0f);
  ASSERT_EQ(mv(0, 0).get_offset_y(), 0.0f);
  mv.set_next_origin();
  ASSERT_EQ(mv(0, 100).get_offset_y(), 0.0f);
  ASSERT_EQ(mv(0, 75).get_offset_y(), 25.0f);
  ASSERT_EQ(mv(0, 0).get_offset_y(), 100.0f);
  ASSERT_EQ(mv(0, 50).get_offset_y(), 50.0f);
  ASSERT_EQ(mv(0, 33).get_offset_y(), 67.0f);
}

TEST(Movement, SensitivityTest) {
  game::Movement mv(3.5f);
  mv(5, 10);
  const gl::math::ViewMatrix &kMat = mv(9, 4);
  ASSERT_NEAR(kMat.get_offset_x(), 4.0f * 3.5f, 0.0001f);
  ASSERT_NEAR(kMat.get_offset_y(), 6.0f * 3.5f, 0.0001f);
}
