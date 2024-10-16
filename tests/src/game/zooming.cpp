#include "game/zooming.hpp"

#include <cmath>
#include <gtest/gtest.h>

static void
_assert_scale(game::Zooming &zooming, bool scale, float expected_val) noexcept {
  zooming(0, 0, scale);
  ASSERT_NEAR(zooming.get().scale, expected_val, 0.0001f);
}

TEST(Zooming, ScaleTest) {
  game::Zooming zoom(0.1f, -INFINITY, INFINITY);

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

TEST(Zooming, ClampTest) {
  game::Zooming zoom(0.2f, 0.6f, 1.5f);

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

TEST(Zooming, OffsetTest) {
  game::Zooming zoom(1.0f, 0.2f, 100.0f);

  zoom(5, 5, true);
  ASSERT_NEAR(zoom.get().scale, 2.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, -5.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, -5.0f, 0.0001f);
  zoom(5, 5, true);
  ASSERT_NEAR(zoom.get().scale, 3.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, -10.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, -10.0f, 0.0001f);
  zoom(4, 6, true);
  ASSERT_NEAR(zoom.get().scale, 4.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, -12.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, -18.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(zoom.get().scale, 3.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, -10.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, -12.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(zoom.get().scale, 2.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, -5.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, -6.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(zoom.get().scale, 1.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, 0.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, 0.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(zoom.get().scale, 0.2f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetX, 4.0f, 0.0001f);
  ASSERT_NEAR(zoom.get().offsetY, 4.8f, 0.0001f);
}
