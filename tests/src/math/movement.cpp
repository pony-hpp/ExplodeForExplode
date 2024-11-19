#include "math/movement.hpp"

#include <gtest/gtest.h>

using namespace math;

static void _assert_mv(
  Movement &mv, float x, float y, const MovementOffset &expectedVal
) noexcept
{
  mv(x, y);
  ASSERT_NEAR(mv.get().x, expectedVal.x, 0.0001f);
  ASSERT_NEAR(mv.get().y, expectedVal.y, 0.0001f);
}

TEST(Movement, MoveXTest)
{
  Movement mv(1.0f);

  _assert_mv(mv, 50.0f, 0.0f, {0.0f, 0.0f});
  _assert_mv(mv, 25.5f, 0.0f, {24.5f, 0.0f});
  _assert_mv(mv, 75.0f, 0.0f, {-25.0f, 0.0f});
  _assert_mv(mv, 99.9f, 0.0f, {-49.9f, 0.0f});
  _assert_mv(mv, 0.0f, 0.0f, {50.0f, 0.0f});

  mv.reset_origin();
  _assert_mv(mv, 0.0f, 0.0f, {50.0f, 0.0f});

  _assert_mv(mv, 10.3f, 0.0f, {39.7f, 0.0f});
  _assert_mv(mv, 25.0f, 0.0f, {25.0f, 0.0f});
  _assert_mv(mv, 50.0f, 0.0f, {0.0f, 0.0f});
  _assert_mv(mv, 0.0f, 0.0f, {50.0f, 0.0f});
}

TEST(Movement, MoveYTest)
{
  Movement mv(1.0f);

  _assert_mv(mv, 0.0f, 0.0f, {0.0f, 0.0f});
  _assert_mv(mv, 0.0f, 33.0f, {0.0f, 33.0f});
  _assert_mv(mv, 0.0f, 75.1f, {0.0f, 75.1f});
  _assert_mv(mv, 0.0f, 100.0f, {0.0f, 100.0f});
  _assert_mv(mv, 0.0f, 0.0f, {0.0f, 0.0f});

  mv.reset_origin();
  _assert_mv(mv, 0.0f, 100.0f, {0.0f, 0.0f});

  _assert_mv(mv, 0.0f, 75.0f, {0.0f, -25.0f});
  _assert_mv(mv, 0.0f, 0.0f, {0.0f, -100.0f});
  _assert_mv(mv, 0.0f, 50.0f, {0.0f, -50.0f});
  _assert_mv(mv, 0.0f, 32.8f, {0.0f, -67.2f});
}

TEST(Movement, SensitivityTest)
{
  Movement mv(3.5f);
  mv(5.0f, 10.0f);
  _assert_mv(mv, 9.0f, 4.0f, {-(4.0f * 3.5f), -(6.0f * 3.5f)});
}
