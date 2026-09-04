#include <cmath>
#include <gtest/gtest.h>
#include <iomanip>
#include <limits>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <numbers>
#include <ostream>
#include "gtest/gtest.h"

using msc::core::maths::Angle;
using msc::core::coordinates::RightAscension;
using msc::core::coordinates::RightAscensionError;
using std::numbers::pi;

namespace
{
    constexpr auto right_ascension =
        RightAscension::try_from(Angle::from_hours(5.5));
    static_assert(right_ascension.has_value());

    constexpr auto invalid_right_ascension =
        RightAscension::try_from(Angle::from_radian(2.0 * pi));
    static_assert(!invalid_right_ascension.has_value());
    static_assert(invalid_right_ascension.error() ==
                  RightAscensionError::OutOfRange);

    struct RightAscensionCase
    {
        Angle value;
    };

    class ValidRightAscensionTest
        : public ::testing::TestWithParam<RightAscensionCase>
    {};

    class InvalidRightAscensionOutOfRangeTest
        : public ::testing::TestWithParam<RightAscensionCase>
    {};

    class InvalidRightAscensionNonFiniteValuesTest
        : public ::testing::TestWithParam<RightAscensionCase>
    {};

    void PrintTo(const RightAscensionCase& tcase, std::ostream* os)
    {
        const auto val = tcase.value.to_radian();
        // how to print precise value of radiant to xth digit?
        *os << "value: " << tcase.value.to_hours() << " rad (= "
            << std::setprecision(std::numeric_limits<double>::max_digits10)
            << val << ")";
    }
} // namespace

TEST_P(ValidRightAscensionTest, TestsValidRightAscension)
{
    const auto& tcase = GetParam();
    const auto ra = RightAscension::try_from(tcase.value);
    ASSERT_TRUE(ra.has_value());
    EXPECT_EQ(ra->get_angle().to_radian(), tcase.value.to_radian());
}

TEST_P(InvalidRightAscensionNonFiniteValuesTest,
       TestsInvalidRightAscensionNonFiniteValues)
{
    const auto& tcase = GetParam();
    const auto ra = RightAscension::try_from(tcase.value);

    ASSERT_FALSE(ra.has_value());
    EXPECT_EQ(ra.error(), RightAscensionError::NonFinite);
}

TEST_P(InvalidRightAscensionOutOfRangeTest,
       TestsInvalidRightAscensionOutOfRange)
{
    const auto& tcase = GetParam();
    const auto ra = RightAscension::try_from(tcase.value);

    ASSERT_FALSE(ra.has_value());
    EXPECT_EQ(ra.error(), RightAscensionError::OutOfRange);
}

INSTANTIATE_TEST_SUITE_P(
    ValidTests, ValidRightAscensionTest,
    ::testing::Values(
        RightAscensionCase{Angle::from_radian(0)},
        RightAscensionCase{Angle::from_radian(-0.0)},
        RightAscensionCase{Angle::from_radian(pi)},
        RightAscensionCase{Angle::from_radian(pi / 2)},
        RightAscensionCase{Angle::from_radian(4 * pi / 3)},
        RightAscensionCase{Angle::from_radian(std::nextafter(0, pi))},
        RightAscensionCase{Angle::from_radian(std::nextafter(2 * pi, 0))},
        RightAscensionCase{Angle::from_radian(7.0 / 3.0)}));

INSTANTIATE_TEST_SUITE_P(
    NonFiniteValuesTests, InvalidRightAscensionNonFiniteValuesTest,
    ::testing::Values(
        RightAscensionCase{
            Angle::from_radian(std::numeric_limits<double>::infinity())},
        RightAscensionCase{
            Angle::from_radian(-std::numeric_limits<double>::infinity())},
        RightAscensionCase{
            Angle::from_radian(std::numeric_limits<double>::quiet_NaN())}));

INSTANTIATE_TEST_SUITE_P(
    OutOfRange, InvalidRightAscensionOutOfRangeTest,
    ::testing::Values(
        RightAscensionCase{Angle::from_radian(2 * pi)},
        RightAscensionCase{Angle::from_radian(-pi)},
        RightAscensionCase{Angle::from_radian(std::nextafter(0, -1))},
        RightAscensionCase{Angle::from_radian(std::nextafter(2 * pi, 4 * pi))}


        ));
