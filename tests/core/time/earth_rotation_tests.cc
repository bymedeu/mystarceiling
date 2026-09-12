#include <gtest/gtest.h>

#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>

#include <numbers>

namespace {
using msc::core::time::earth_rotation_angle;
using msc::core::time::JulianDate;

constexpr double sofa_reference_era = 0.4022837240028158102;
constexpr double reference_tolerance = 1e-12;
} // namespace

TEST(EarthRotationTest, MatchesSofaReferenceValue) {
  const auto era = earth_rotation_angle(JulianDate{2400000.5, 54388.0});

  EXPECT_NEAR(era.to_radian(), sofa_reference_era, reference_tolerance);
}

TEST(EarthRotationTest, IsIndependentOfJulianDateSplitAndPartOrder) {
  const auto reference = earth_rotation_angle(JulianDate{2400000.5, 54388.25});
  const auto j2000_split = earth_rotation_angle(JulianDate{2451545.0, 2843.75});
  const auto reversed_parts =
      earth_rotation_angle(JulianDate{54388.25, 2400000.5});

  EXPECT_NEAR(j2000_split.to_radian(), reference.to_radian(),
              reference_tolerance);
  EXPECT_NEAR(reversed_parts.to_radian(), reference.to_radian(),
              reference_tolerance);
}

TEST(EarthRotationTest, ReturnsCanonicalAngle) {
  const auto era = earth_rotation_angle(JulianDate{2451545.0, -1000000.0});

  EXPECT_GE(era.to_radian(), 0.0);
  EXPECT_LT(era.to_radian(), 2.0 * std::numbers::pi);
}
