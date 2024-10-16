#include "game/movement.hpp"

#include <gtest/gtest.h>

TEST(Movement, MoveXTest) {
  game::Movement mv(1.0f);

  mv(50, 0); // Set origin.
  ASSERT_EQ(mv.get().x, 0.0f);
  mv(25, 0);
  ASSERT_EQ(mv.get().x, -25.0f);
  mv(75, 0);
  ASSERT_EQ(mv.get().x, 25.0f);
  mv(100, 0);
  ASSERT_EQ(mv.get().x, 50.0f);
  mv(0.0, 0);
  ASSERT_EQ(mv.get().x, -50.0f);

  mv.set_next_origin();

  mv(0, 0);
  ASSERT_EQ(mv.get().x, -50.0f);
  mv(10, 0);
  ASSERT_EQ(mv.get().x, -40.0f);
  mv(25, 0);
  ASSERT_EQ(mv.get().x, -25.0f);
  mv(50, 0);
  ASSERT_EQ(mv.get().x, 0.0f);
  mv(0, 0);
  ASSERT_EQ(mv.get().x, -50.0f);
}

TEST(Movement, MoveYTest) {
  game::Movement mv(1.0f);

  mv(0, 0);
  ASSERT_EQ(mv.get().y, 0.0f);
  mv(0, 33);
  ASSERT_EQ(mv.get().y, -33.0f); // Note that the Y direction is inverted.
  mv(0, 75);
  ASSERT_EQ(mv.get().y, -75.0f);
  mv(0, 100);
  ASSERT_EQ(mv.get().y, -100.0f);
  mv(0, 0);
  ASSERT_EQ(mv.get().y, 0.0f);

  mv.set_next_origin();

  mv(0, 100);
  ASSERT_EQ(mv.get().y, 0.0f);
  mv(0, 75);
  ASSERT_EQ(mv.get().y, 25.0f);
  mv(0, 0);
  ASSERT_EQ(mv.get().y, 100.0f);
  mv(0, 50);
  ASSERT_EQ(mv.get().y, 50.0f);
  mv(0, 33);
  ASSERT_EQ(mv.get().y, 67.0f);
}

TEST(Movement, SensitivityTest) {
  game::Movement mv(3.5f);
  mv(5, 10);
  mv(9, 4);
  ASSERT_NEAR(mv.get().x, 4.0f * 3.5f, 0.0001f);
  ASSERT_NEAR(mv.get().y, 6.0f * 3.5f, 0.0001f);
}
