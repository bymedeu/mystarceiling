#include <gtest/gtest.h>

#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>

using msc::core::coordinates::Latitude;
using msc::core::coordinates::Longitude;
using msc::core::coordinates::ObserverLocation;
using msc::core::maths::Angle;

namespace {
constexpr auto latitude = Latitude::try_from(Angle::from_degrees(43.6045));
constexpr auto longitude = Longitude::try_from(Angle::from_degrees(1.4440));
static_assert(latitude.has_value());
static_assert(longitude.has_value());

constexpr ObserverLocation toulouse{*latitude, *longitude};
static_assert(toulouse.get_latitude().get_angle().to_radian() ==
              latitude->get_angle().to_radian());
static_assert(toulouse.get_longitude().get_angle().to_radian() ==
              longitude->get_angle().to_radian());
} // namespace

TEST(ObserverLocationTest, PreservesCoordinatesThroughConstAccessors) {
  const auto latitude = Latitude::try_from(Angle::from_degrees(-33.8688));
  const auto longitude = Longitude::try_from(Angle::from_degrees(151.2093));

  ASSERT_TRUE(latitude.has_value());
  ASSERT_TRUE(longitude.has_value());

  const ObserverLocation sydney{*latitude, *longitude};

  EXPECT_DOUBLE_EQ(sydney.get_latitude().get_angle().to_radian(),
                   latitude->get_angle().to_radian());
  EXPECT_DOUBLE_EQ(sydney.get_longitude().get_angle().to_radian(),
                   longitude->get_angle().to_radian());
}
