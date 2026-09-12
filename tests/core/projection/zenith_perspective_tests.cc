#include <gtest/gtest.h>

#include <mystarceiling/core/projection/zenith_perspective.hh>

using msc::core::maths::Vector3;
using msc::core::projection::project_zenith_perspective;

TEST(ZenithPerspectiveTest, PlacesZenithAtCenter) {
  const auto projected =
      project_zenith_perspective(Vector3{0, 0, 1}, 16.0 / 9.0);
  ASSERT_TRUE(projected.has_value());
  EXPECT_DOUBLE_EQ(projected->x, 0.0);
  EXPECT_DOUBLE_EQ(projected->y, 0.0);
}

TEST(ZenithPerspectiveTest, PlacesNorthAtTopAndEastAtRight) {
  const auto north = project_zenith_perspective(Vector3{0, 0.5, 1}, 1.0);
  const auto east = project_zenith_perspective(Vector3{0.5, 0, 1}, 1.0);
  ASSERT_TRUE(north && east);
  EXPECT_GT(north->y, 0.0);
  EXPECT_GT(east->x, 0.0);
}

TEST(ZenithPerspectiveTest, RejectsBelowHorizonAndOutsideView) {
  EXPECT_FALSE(project_zenith_perspective(Vector3{0, 0, -1}, 1.0));
  EXPECT_FALSE(project_zenith_perspective(Vector3{0, 2, 1}, 1.0));
}
