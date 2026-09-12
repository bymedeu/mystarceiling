#include <gtest/gtest.h>

#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>

#include <limits>
#include <ostream>

using msc::core::coordinates::Declination;
using msc::core::coordinates::icrs_to_enu;
using msc::core::coordinates::IcrsDirection;
using msc::core::coordinates::Latitude;
using msc::core::coordinates::Longitude;
using msc::core::coordinates::ObserverLocation;
using msc::core::coordinates::RightAscension;
using msc::core::maths::Angle;

namespace {
constexpr double floating_point_tolerance =
    128.0 * std::numeric_limits<double>::epsilon();

struct EnuCase {
  double right_ascension_hours;
  double declination_degrees;
  double latitude_degrees;
  double longitude_degrees;
  double greenwich_sidereal_hours;
  double expected_east;
  double expected_north;
  double expected_up;
};

class IcrsToEnuTest : public ::testing::TestWithParam<EnuCase> {};

void PrintTo(const EnuCase &test_case, std::ostream *output) {
  *output << "RA=" << test_case.right_ascension_hours
          << " h, dec=" << test_case.declination_degrees
          << " deg, lat=" << test_case.latitude_degrees
          << " deg, lon=" << test_case.longitude_degrees
          << " deg, GSA=" << test_case.greenwich_sidereal_hours << " h";
}
} // namespace

TEST_P(IcrsToEnuTest, UsesDocumentedEastNorthUpConvention) {
  const auto &test_case = GetParam();
  const auto right_ascension = RightAscension::try_from(
      Angle::from_hours(test_case.right_ascension_hours));
  const auto declination =
      Declination::try_from(Angle::from_degrees(test_case.declination_degrees));
  const auto latitude =
      Latitude::try_from(Angle::from_degrees(test_case.latitude_degrees));
  const auto longitude =
      Longitude::try_from(Angle::from_degrees(test_case.longitude_degrees));

  ASSERT_TRUE(right_ascension.has_value());
  ASSERT_TRUE(declination.has_value());
  ASSERT_TRUE(latitude.has_value());
  ASSERT_TRUE(longitude.has_value());

  const auto enu =
      icrs_to_enu(IcrsDirection{*right_ascension, *declination},
                  ObserverLocation{*latitude, *longitude},
                  Angle::from_hours(test_case.greenwich_sidereal_hours));

  EXPECT_NEAR(enu.x(), test_case.expected_east, floating_point_tolerance);
  EXPECT_NEAR(enu.y(), test_case.expected_north, floating_point_tolerance);
  EXPECT_NEAR(enu.z(), test_case.expected_up, floating_point_tolerance);
}

TEST(IcrsToEnuTest, PreservesUnitLengthForToulouseObserver) {
  const auto right_ascension = RightAscension::try_from(Angle::from_hours(5.5));
  const auto declination = Declination::try_from(Angle::from_degrees(-23.75));
  const auto latitude = Latitude::try_from(Angle::from_degrees(43.6045));
  const auto longitude = Longitude::try_from(Angle::from_degrees(1.4440));

  ASSERT_TRUE(right_ascension.has_value());
  ASSERT_TRUE(declination.has_value());
  ASSERT_TRUE(latitude.has_value());
  ASSERT_TRUE(longitude.has_value());

  const auto enu = icrs_to_enu(IcrsDirection{*right_ascension, *declination},
                               ObserverLocation{*latitude, *longitude},
                               Angle::from_hours(10.25));
  const auto squared_length =
      enu.x() * enu.x() + enu.y() * enu.y() + enu.z() * enu.z();

  EXPECT_NEAR(squared_length, 1.0, floating_point_tolerance);
}

INSTANTIATE_TEST_SUITE_P(
    ReferenceDirections, IcrsToEnuTest,
    ::testing::Values(EnuCase{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
                      EnuCase{6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                      EnuCase{18.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0},
                      EnuCase{12.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
                      EnuCase{0.0, 90.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                      EnuCase{6.0, 0.0, 0.0, 90.0, 0.0, 0.0, 0.0, 1.0}));
