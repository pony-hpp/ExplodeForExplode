#include "game/movement.hpp"

#include <gtest/gtest.h>

TEST(Movement, MoveXTest) {
  game::Movement mv(1.0f);
  ASSERT_EQ(mv(50, 0).x, 0.0f); // Set origin.
  ASSERT_EQ(mv(25, 0).x, -25.0f);
  ASSERT_EQ(mv(75, 0).x, 25.0f);
  ASSERT_EQ(mv(100, 0).x, 50.0f);
  ASSERT_EQ(mv(0.0, 0).x, -50.0f);
  mv.set_next_origin();
  ASSERT_EQ(mv(0, 0).x, -50.0f);
  ASSERT_EQ(mv(10, 0).x, -40.0f);
  ASSERT_EQ(mv(25, 0).x, -25.0f);
  ASSERT_EQ(mv(50, 0).x, 0.0f);
  ASSERT_EQ(mv(0, 0).x, -50.0f);
}

TEST(Movement, MoveYTest) {
  game::Movement mv(1.0f);
  ASSERT_EQ(mv(0, 0).y, 0.0f);
  // Note that the Y direction is inverted.
  ASSERT_EQ(mv(0, 33).y, -33.0f);
  ASSERT_EQ(mv(0, 75).y, -75.0f);
  ASSERT_EQ(mv(0, 100).y, -100.0f);
  ASSERT_EQ(mv(0, 0).y, 0.0f);
  mv.set_next_origin();
  ASSERT_EQ(mv(0, 100).y, 0.0f);
  ASSERT_EQ(mv(0, 75).y, 25.0f);
  ASSERT_EQ(mv(0, 0).y, 100.0f);
  ASSERT_EQ(mv(0, 50).y, 50.0f);
  ASSERT_EQ(mv(0, 33).y, 67.0f);
}

TEST(Movement, SensitivityTest) {
  game::Movement mv(3.5f);
  mv(5, 10);
  const game::MovementOffset &kOffset = mv(9, 4);
  ASSERT_NEAR(kOffset.x, 4.0f * 3.5f, 0.0001f);
  ASSERT_NEAR(kOffset.y, 6.0f * 3.5f, 0.0001f);
}
