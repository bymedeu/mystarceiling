#include <gtest/gtest.h>

#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/maths/angle.hh>

#include <cmath>
#include <iomanip>
#include <limits>
#include <numbers>
#include <ostream>

using mystarceiling::core::coordinates::Latitude;
using mystarceiling::core::coordinates::LatitudeError;
using mystarceiling::core::maths::Angle;

namespace
{
    struct LatitudeCase
    {
        Angle angle;
    };

    void PrintTo(const LatitudeCase& test_case, std::ostream* output)
    {
        *output << std::setprecision(std::numeric_limits<double>::max_digits10)
                << test_case.angle.to_degrees() << " deg (= "
                << test_case.angle.to_radian() << " rad)";
    }

    constexpr auto latitude =
        Latitude::try_from(Angle::from_degrees(48.8566));
    static_assert(latitude.has_value());

    constexpr auto invalid_latitude =
        Latitude::try_from(Angle::from_degrees(91.0));
    static_assert(!invalid_latitude.has_value());
    static_assert(invalid_latitude.error() == LatitudeError::OutOfRange);

    class ValidLatitudeTest : public ::testing::TestWithParam<LatitudeCase>
    {};

    class OutOfRangeLatitudeTest
        : public ::testing::TestWithParam<LatitudeCase>
    {};

    class NonFiniteLatitudeTest
        : public ::testing::TestWithParam<LatitudeCase>
    {};
} // namespace

TEST_P(ValidLatitudeTest, PreservesValidAngle)
{
    const auto angle = GetParam().angle;
    const auto latitude = Latitude::try_from(angle);

    ASSERT_TRUE(latitude.has_value());
    EXPECT_DOUBLE_EQ(latitude->get_angle().to_radian(), angle.to_radian());
}

TEST_P(OutOfRangeLatitudeTest, RejectsAngleOutsideClosedRange)
{
    const auto latitude = Latitude::try_from(GetParam().angle);

    ASSERT_FALSE(latitude.has_value());
    EXPECT_EQ(latitude.error(), LatitudeError::OutOfRange);
}

TEST_P(NonFiniteLatitudeTest, RejectsNonFiniteAngle)
{
    const auto latitude = Latitude::try_from(GetParam().angle);

    ASSERT_FALSE(latitude.has_value());
    EXPECT_EQ(latitude.error(), LatitudeError::NonFinite);
}

INSTANTIATE_TEST_SUITE_P(
    ValidLatitudes, ValidLatitudeTest,
    ::testing::Values(
        LatitudeCase{Angle::from_degrees(0.0)},
        LatitudeCase{Angle::from_degrees(45.0)},
        LatitudeCase{Angle::from_degrees(-45.0)},
        LatitudeCase{Angle::from_degrees(90.0)},
        LatitudeCase{Angle::from_degrees(-90.0)},
        LatitudeCase{Angle::from_radian(
            std::nextafter(std::numbers::pi / 2, 0.0))},
        LatitudeCase{Angle::from_radian(
            std::nextafter(-std::numbers::pi / 2, 0.0))}));

INSTANTIATE_TEST_SUITE_P(
    InvalidLatitudes, OutOfRangeLatitudeTest,
    ::testing::Values(
        LatitudeCase{Angle::from_radian(std::nextafter(
            std::numbers::pi / 2, std::numbers::pi))},
        LatitudeCase{Angle::from_radian(std::nextafter(
            -std::numbers::pi / 2, -std::numbers::pi))}));

INSTANTIATE_TEST_SUITE_P(
    NonFiniteLatitudes, NonFiniteLatitudeTest,
    ::testing::Values(
        LatitudeCase{
            Angle::from_radian(std::numeric_limits<double>::infinity())},
        LatitudeCase{
            Angle::from_radian(-std::numeric_limits<double>::infinity())},
        LatitudeCase{Angle::from_radian(
            std::numeric_limits<double>::quiet_NaN())}));
