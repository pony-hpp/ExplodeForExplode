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

static void _assert_projection(
  float maxX, float maxY, float expectedScaleX, float expectedScaleY
) noexcept {
  _assert_matrices(
    ProjectionMatrix(maxX, maxY),
    {
      // clang-format off
      expectedScaleX, 0.0f,           0.0f, -1.0f,
      0.0f,           expectedScaleY, 0.0f, -1.0f,
      0.0f,           0.0f,           1.0f,  0.0f,
      0.0f,           0.0f,           0.0f,  1.0f
      // clang-format on
    }
  );
}

TEST(GlMath, ProjectionMatrixTest) {
  _assert_projection(40, 40, 0.05f, 0.05f);
  _assert_projection(800, 600, 0.0025f, 0.0033f);
  _assert_projection(750, 1334, 0.0026f, 0.0014f);
}

static void _assert_offset(ViewMatrix &mat, float x, float y) noexcept {
  mat.set_offset(x, y);
  ASSERT_EQ(mat.get_offset_x(), x);
  ASSERT_EQ(mat.get_offset_y(), y);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, x,
      0.0f, 1.0f, 0.0f, y,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      // clang-format on
    }
  );
}

TEST(GlMath, ViewMatrixOffsetTest) {
  ViewMatrix mat;
  _assert_offset(mat, 0.5f, 0.5f);
  _assert_offset(mat, -0.5f, 0.5f);
  _assert_offset(mat, 0.5f, -0.5f);
  _assert_offset(mat, -0.5f, -0.5f);
  _assert_offset(mat, 0.0f, 0.0f);
  _assert_offset(mat, 1.0f, 0.0f);
  _assert_offset(mat, .0f, 1.0f);
  _assert_offset(mat, 1.0f, 1.0f);
}

static void _assert_scale(ViewMatrix &mat, float scale) noexcept {
  mat.set_scale(scale);
  ASSERT_EQ(mat.get_scale(), scale);
  _assert_matrices(
    mat,
    {
      // clang-format off
      scale, 0.0f,  0.0f, 0.0f,
      0.0f,  scale, 0.0f, 0.0f,
      0.0f,  0.0f,  1.0f, 0.0f,
      0.0f,  0.0f,  0.0f, 1.0f
      // clang-format on
    }
  );
}

TEST(GlMath, ViewMatrixScaleTest) {
  ViewMatrix mat;
  _assert_scale(mat, 0.5f);
  _assert_scale(mat, 0.0f);
  _assert_scale(mat, -0.5f);
  _assert_scale(mat, 1.0f);
  _assert_scale(mat, -1.0f);
  _assert_scale(mat, 0.25f);
  _assert_scale(mat, -0.25f);
}
