#include <gtest/gtest.h>

#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/maths/angle.hh>

#include <cmath>
#include <iomanip>
#include <limits>
#include <numbers>
#include <ostream>

using msc::core::coordinates::Longitude;
using msc::core::coordinates::LongitudeError;
using msc::core::maths::Angle;

namespace
{
    struct LongitudeCase
    {
        Angle angle;
    };

    void PrintTo(const LongitudeCase& test_case, std::ostream* output)
    {
        *output << std::setprecision(std::numeric_limits<double>::max_digits10)
                << test_case.angle.to_degrees() << " deg (= "
                << test_case.angle.to_radian() << " rad)";
    }

    constexpr auto longitude =
        Longitude::try_from(Angle::from_degrees(2.3522));
    static_assert(longitude.has_value());

    constexpr auto invalid_longitude =
        Longitude::try_from(Angle::from_degrees(180.0));
    static_assert(!invalid_longitude.has_value());
    static_assert(invalid_longitude.error() == LongitudeError::OutOfRange);

    class ValidLongitudeTest
        : public ::testing::TestWithParam<LongitudeCase>
    {};

    class OutOfRangeLongitudeTest
        : public ::testing::TestWithParam<LongitudeCase>
    {};

    class NonFiniteLongitudeTest
        : public ::testing::TestWithParam<LongitudeCase>
    {};
} // namespace

TEST_P(ValidLongitudeTest, PreservesAngleInCanonicalRange)
{
    const auto angle = GetParam().angle;
    const auto longitude = Longitude::try_from(angle);

    ASSERT_TRUE(longitude.has_value());
    EXPECT_DOUBLE_EQ(longitude->get_angle().to_radian(), angle.to_radian());
}

TEST_P(OutOfRangeLongitudeTest, RejectsAngleOutsideCanonicalRange)
{
    const auto longitude = Longitude::try_from(GetParam().angle);

    ASSERT_FALSE(longitude.has_value());
    EXPECT_EQ(longitude.error(), LongitudeError::OutOfRange);
}

TEST_P(NonFiniteLongitudeTest, RejectsNonFiniteAngle)
{
    const auto longitude = Longitude::try_from(GetParam().angle);

    ASSERT_FALSE(longitude.has_value());
    EXPECT_EQ(longitude.error(), LongitudeError::NonFinite);
}

INSTANTIATE_TEST_SUITE_P(
    ValidLongitudes, ValidLongitudeTest,
    ::testing::Values(
        LongitudeCase{Angle::from_degrees(0.0)},
        LongitudeCase{Angle::from_degrees(45.0)},
        LongitudeCase{Angle::from_degrees(-45.0)},
        LongitudeCase{Angle::from_radian(-std::numbers::pi)},
        LongitudeCase{Angle::from_radian(
            std::nextafter(std::numbers::pi, 0.0))},
        LongitudeCase{Angle::from_radian(
            std::nextafter(-std::numbers::pi, 0.0))}));

INSTANTIATE_TEST_SUITE_P(
    InvalidLongitudes, OutOfRangeLongitudeTest,
    ::testing::Values(
        LongitudeCase{Angle::from_radian(std::numbers::pi)},
        LongitudeCase{Angle::from_radian(
            std::nextafter(std::numbers::pi,
                           std::numeric_limits<double>::infinity()))},
        LongitudeCase{Angle::from_radian(
            std::nextafter(-std::numbers::pi,
                           -std::numeric_limits<double>::infinity()))}));

INSTANTIATE_TEST_SUITE_P(
    NonFiniteLongitudes, NonFiniteLongitudeTest,
    ::testing::Values(
        LongitudeCase{
            Angle::from_radian(std::numeric_limits<double>::infinity())},
        LongitudeCase{
            Angle::from_radian(-std::numeric_limits<double>::infinity())},
        LongitudeCase{Angle::from_radian(
            std::numeric_limits<double>::quiet_NaN())}));
