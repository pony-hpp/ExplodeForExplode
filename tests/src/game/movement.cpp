#include "game/movement.hpp"

#include <gtest/gtest.h>

using namespace game;

static void _assert_mv(
  Movement &mv, long long x, long long y, const MovementOffset &expectedVal
) noexcept
{
  mv(x, y);
  ASSERT_NEAR(mv.get().x, expectedVal.x, 0.0001f);
  ASSERT_NEAR(mv.get().y, expectedVal.y, 0.0001f);
}

TEST(Movement, MoveXTest)
{
  Movement mv(1.0f);

  _assert_mv(mv, 50, 0, {-0.0f, 0.0f}); // Set origin.
  _assert_mv(mv, 25, 0, {25.0f, 0.0f});
  _assert_mv(mv, 75, 0, {-25.0f, 0.0f});
  _assert_mv(mv, 100, 0, {-50.0f, 0.0f});
  _assert_mv(mv, 0, 0, {50.0f, 0.0f});

  mv.set_next_origin(); // Set next point as origin.

  _assert_mv(mv, 0, 0, {50.0f, 0.0f});
  _assert_mv(mv, 10, 0, {40.0f, 0.0f});
  _assert_mv(mv, 25, 0, {25.0f, 0.0f});
  _assert_mv(mv, 50, 0, {0.0f, 0.0f});
  _assert_mv(mv, 0, 0, {50.0f, 0.0f});
}

TEST(Movement, MoveYTest)
{
  Movement mv(1.0f);

  _assert_mv(mv, 0, 0, {0.0f, 0.0f});
  _assert_mv(mv, 0, 33, {0.0f, 33.0f});
  _assert_mv(mv, 0, 75, {0.0f, 75.0f});
  _assert_mv(mv, 0, 100, {0.0f, 100.0f});
  _assert_mv(mv, 0, 0, {0.0f, 0.0f});

  mv.set_next_origin();

  _assert_mv(mv, 0, 100, {0.0f, 0.0f});
  _assert_mv(mv, 0, 75, {0.0f, -25.0f});
  _assert_mv(mv, 0, 0, {0.0f, -100.0f});
  _assert_mv(mv, 0, 50, {0.0f, -50.0f});
  _assert_mv(mv, 0, 33, {0.0f, -67.0f});
}

TEST(Movement, SensitivityTest)
{
  Movement mv(3.5f);
  mv(5, 10);
  _assert_mv(mv, 9, 4, {-(4.0f * 3.5f), -(6.0f * 3.5f)});
}
