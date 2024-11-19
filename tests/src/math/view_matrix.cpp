#include "math/matrix/matrix.hpp"

#include <gtest/gtest.h>

using namespace math;

static void _assert_matrices(const Matrix &v1, const Matrix &v2) noexcept
{
  for (byte y = 0; y < 4; y++)
  {
    for (byte x = 0; x < 4; x++)
    {
      ASSERT_NEAR(v1[y][x], v2[y][x], 0.0001f);
    }
  }
}

TEST(ViewMatrix, OffsetTest)
{
  ViewMatrix view;

  view.set_offset(0.0f, 0.0f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            // clang-format on
          })
  );

  view.set_offset(0.0f, 3.14f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            1.0f, 0.0f, 0.0f,  0.0f,
            0.0f, 1.0f, 0.0f, -3.14f,
            0.0f, 0.0f, 1.0f,  0.0f,
            0.0f, 0.0f, 0.0f,  1.0f
            // clang-format on
          })
  );

  view.set_offset(-0.25f, 0.0f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            1.0f, 0.0f, 0.0f, 0.25f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            // clang-format on
          })
  );

  view.set_offset(0.5f, -285.582f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            1.0f, 0.0f, 0.0f, -0.5f,
            0.0f, 1.0f, 0.0f,  285.582f,
            0.0f, 0.0f, 1.0f,  0.0f,
            0.0f, 0.0f, 0.0f,  1.0f
            // clang-format on
          })
  );
}

TEST(ViewMatrix, ScaleTest)
{
  ViewMatrix view;

  view.set_scale(1.0f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            // clang-format on
          })
  );

  view.set_scale(0.0f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            // clang-format on
          })
  );

  view.set_scale(0.2024f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            0.2024f, 0.0f,    0.0f, 0.0f,
            0.0f,    0.2024f, 0.0f, 0.0f,
            0.0f,    0.0f,    1.0f, 0.0f,
            0.0f,    0.0f,    0.0f, 1.0f
            // clang-format on
          })
  );

  view.set_scale(-2.71f);
  _assert_matrices(
    view, Matrix({
            // clang-format off
            -2.71f, 0.0f,  0.0f, 0.0f,
             0.0f, -2.71f, 0.0f, 0.0f,
             0.0f,  0.0f,  1.0f, 0.0f,
             0.0f,  0.0f,  0.0f, 1.0f
            // clang-format on
          })
  );
}
