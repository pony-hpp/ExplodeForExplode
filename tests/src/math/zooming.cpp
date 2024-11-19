#include "math/zooming.hpp"

#include <cmath>
#include <gtest/gtest.h>

using namespace math;

static void
_assert_scale(Zooming &zooming, bool scale, float expectedVal) noexcept
{
  zooming(scale, 0.0f, 0.0f);
  ASSERT_NEAR(zooming.get().scale, expectedVal, 0.0001f);
}

TEST(Zooming, ScaleTest)
{
  Zooming zoom(0.1f, INFINITY, -INFINITY);

  _assert_scale(zoom, true, 1.1f);
  _assert_scale(zoom, true, 1.2f);
  _assert_scale(zoom, true, 1.3f);
  _assert_scale(zoom, false, 1.2f);
  _assert_scale(zoom, true, 1.3f);
  _assert_scale(zoom, true, 1.4f);
  _assert_scale(zoom, true, 1.5f);
  _assert_scale(zoom, false, 1.4f);
  _assert_scale(zoom, false, 1.3f);
  _assert_scale(zoom, false, 1.2f);
  _assert_scale(zoom, false, 1.1f);
  _assert_scale(zoom, false, 1.0f);
  _assert_scale(zoom, false, 0.9f);
  _assert_scale(zoom, false, 0.8f);
  _assert_scale(zoom, true, 0.9f);
  _assert_scale(zoom, true, 1.0f);
  _assert_scale(zoom, true, 1.1f);
}

TEST(Zooming, ClampTest)
{
  Zooming zoom(0.2f, 1.5f, 0.6f);

  _assert_scale(zoom, true, 1.2f);
  _assert_scale(zoom, true, 1.4f);
  _assert_scale(zoom, true, 1.5f);
  _assert_scale(zoom, true, 1.5f);
  _assert_scale(zoom, false, 1.3f);
  _assert_scale(zoom, false, 1.1f);
  _assert_scale(zoom, false, 0.9f);
  _assert_scale(zoom, false, 0.7f);
  _assert_scale(zoom, false, 0.6f);
  _assert_scale(zoom, false, 0.6f);
  _assert_scale(zoom, true, 0.8f);
  _assert_scale(zoom, false, 0.6f);
  _assert_scale(zoom, false, 0.6f);
}

static void _assert_offset(
  Zooming &zooming, bool scale, float x, float y, float expectedX,
  float expectedY
) noexcept
{
  zooming(scale, x, y);
  ASSERT_NEAR(zooming.get().offsetX, expectedX, 0.0001f);
  ASSERT_NEAR(zooming.get().offsetY, expectedY, 0.0001f);
}

TEST(Zooming, OffsetTest)
{
  Zooming zoom(1.0f, 100.0f, 0.2f);

  _assert_offset(zoom, true, 5.0f, 5.0f, 5.0f, 5.0f);
  _assert_offset(zoom, true, 5.0f, 5.0f, 10.0f, 10.0f);
  _assert_offset(zoom, true, 4.1f, 5.9f, 4.1f * 3.0f, 5.9f * 3.0f);
  _assert_offset(zoom, false, 5.0f, 6.0f, 10.0f, 12.0f);
  _assert_offset(zoom, false, 4.0f, 2.0f, 4.0f, 2.0f);
  _assert_offset(zoom, false, 314.15f, 926.53f, 0.0f, 0.0f);
  _assert_offset(zoom, false, 5.0f, 6.0f, -4.0f, -4.8f);
}
