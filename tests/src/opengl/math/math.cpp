#include "opengl/math/math.hpp"

#include <gtest/gtest.h>

using namespace gl::math;

TEST(GlMath, NormalizeTest) {
  ASSERT_NEAR(normalize(0.0f, 128.0f), 0.0f, 0.01f);
  ASSERT_NEAR(normalize(1.0f, 100.0f), 0.001f, 0.01f);
  ASSERT_NEAR(normalize(2.0f, 50.0f), 0.04f, 0.01f);
  ASSERT_NEAR(normalize(12.5f, 25.0f), 0.5f, 0.01f);
  ASSERT_NEAR(normalize(314.0f, 314.0f), 1.0f, 0.01f);
  ASSERT_NEAR(normalize(542.0f, 271.0f), 2.0f, 0.01f);
}
