#include "game/zooming.hpp"

#include <cmath>
#include <gtest/gtest.h>

TEST(Zooming, ScaleTest) {
  game::Zooming zoom(0.1f, -INFINITY, INFINITY);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.1f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.2f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.3f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.2f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.3f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.4f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.5f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.4f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.3f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.2f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.1f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.0f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.9f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.8f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 0.9f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.0f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.1f, 0.0001f);
}

TEST(Zooming, ClampTest) {
  game::Zooming zoom(0.2f, 0.6f, 1.5f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.2f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.4f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.5f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 1.5f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.3f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 1.1f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.9f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.7f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.6f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.6f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, true).scale, 0.8f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.6f, 0.0001f);
  ASSERT_NEAR(zoom(0, 0, false).scale, 0.6f, 0.0001f);
}

TEST(Zooming, OffsetTest) {
  game::Zooming zoom(1.0f, 0.2f, 100.0f);
  const game::Zoom &kZoom = zoom(5, 5, true);
  ASSERT_NEAR(kZoom.scale, 2.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, -5.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, -5.0f, 0.0001f);
  zoom(5, 5, true);
  ASSERT_NEAR(kZoom.scale, 3.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, -10.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, -10.0f, 0.0001f);
  zoom(4, 6, true);
  ASSERT_NEAR(kZoom.scale, 4.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, -12.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, -18.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(kZoom.scale, 3.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, -10.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, -12.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(kZoom.scale, 2.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, -5.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, -6.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(kZoom.scale, 1.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, 0.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, 0.0f, 0.0001f);
  zoom(5, 6, false);
  ASSERT_NEAR(kZoom.scale, 0.2f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetX, 4.0f, 0.0001f);
  ASSERT_NEAR(kZoom.offsetY, 4.8f, 0.0001f);
}
