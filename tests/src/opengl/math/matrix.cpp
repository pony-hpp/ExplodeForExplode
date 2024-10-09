#include "opengl/math/matrix.hpp"

#include <gtest/gtest.h>

using namespace gl::math;

static void _assert_matrices(const Matrix &mat1, const Matrix &mat2) noexcept {
  for (unsigned char i = 0; i < 16; i++) {
    ASSERT_NEAR(mat1[i], mat2[i], 0.0001f);
  }
}

TEST(GlMath, ProjectionMatrixTest) {
  _assert_matrices(
    projection_matrix(40, 40),
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
    projection_matrix(800, 600),
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
    projection_matrix(750, 1334),
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

TEST(GlMath, TranslateMatrixTest) {
  Matrix mat = DEFAULT_MATRIX;

  translate(mat, 0.5f, 0.5f);
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

  translate(mat, -0.5f, 0.5f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      // clang-format on
    }
  );

  translate(mat, 0.0f, -1.0f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      // clang-format on
    }
  );

  translate(mat, 0.0f, 0.0f);
  _assert_matrices(
    mat,
    {
      // clang-format off
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      // clang-format on
    }
  );
}
