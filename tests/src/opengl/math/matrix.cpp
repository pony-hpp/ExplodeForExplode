#include "opengl/math/matrix.hpp"

#include <gtest/gtest.h>

using namespace gl::math;

static void _assert_matrices(const Matrix &mat1, const Matrix &mat2) noexcept {
  for (unsigned char i = 0; i < 4; i++) {
    for (unsigned char j = 0; j < 4; j++) {
      ASSERT_NEAR(mat1[i][j], mat2[i][j], 0.0001f);
    }
  }
}

TEST(GlMath, ProjectionMatrixTest) {
  _assert_matrices(
    ProjectionMatrix(40, 40),
    {
      // clang-format off
      0.05f, 0.0f,  0.0f, -1.0f,
      0.0f,  0.05f, 0.0f, -1.0f,
      0.0f,  0.0f,  1.0f,  0.0f,
      0.0f,  0.0f,  0.0f,  1.0f
      // clang-format on
    }
  );

  _assert_matrices(
    ProjectionMatrix(800, 600),
    {
      // clang-format off
      0.0025f, 0.0f,   0.0f, -1.0f,
      0.0f,   0.0033f, 0.0f, -1.0f,
      0.0f,   0.0f,    1.0f,  0.0f,
      0.0f,   0.0f,    0.0f,  1.0f
      // clang-format on
    }
  );

  _assert_matrices(
    ProjectionMatrix(750, 1334),
    {
      // clang-format off
      0.0026f, 0.0f,  0.0f, -1.0f,
      0.0f,  0.0014f, 0.0f, -1.0f,
      0.0f,  0.0f,  1.0f,  0.0f,
      0.0f,  0.0f,  0.0f,  1.0f
      // clang-format on
    }
  );
}

TEST(GlMath, ViewMatrixOffsetTest) {
  ViewMatrix mat;

  mat.set_offset(0.5f, 0.5f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, 0.5f,
      0.0f, 1.0f, 0.0f, 0.5f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      // clang-format on
    }
  );
  ASSERT_EQ(mat.get_offset_x(), 0.5f);
  ASSERT_EQ(mat.get_offset_y(), 0.5f);

  mat.set_offset(-0.33f, 0.33f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, -0.33f,
      0.0f, 1.0f, 0.0f,  0.33f,
      0.0f, 0.0f, 1.0f,  0.0f,
      0.0f, 0.0f, 0.0f,  1.0f
      // clang-format on
    }
  );
  ASSERT_EQ(mat.get_offset_x(), -0.33f);
  ASSERT_EQ(mat.get_offset_y(), 0.33f);

  mat.set_offset(0.0f, -0.125f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f,  0.0f,
      0.0f, 1.0f, 0.0f, -0.125f,
      0.0f, 0.0f, 1.0f,  0.0f,
      0.0f, 0.0f, 0.0f,  1.0f
      // clang-format on
    }
  );
  ASSERT_EQ(mat.get_offset_x(), 0.0f);
  ASSERT_EQ(mat.get_offset_y(), -0.125f);

  mat.set_offset(-1.0f, -0.404f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, -1.0f,
      0.0f, 1.0f, 0.0f, -0.404f,
      0.0f, 0.0f, 1.0f,  0.0f,
      0.0f, 0.0f, 0.0f,  1.0f
      // clang-format on
    }
  );
  ASSERT_EQ(mat.get_offset_x(), -1.0f);
  ASSERT_EQ(mat.get_offset_y(), -0.404f);
}
