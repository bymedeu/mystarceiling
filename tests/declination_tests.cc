#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <numbers>
#include <ostream>
#include "gtest/gtest.h"
#include "mystarceiling/core/coordinates/declination.hh"

using mystarceiling::core::maths::Angle;
using mystarceiling::core::coordinates::Declination;
using mystarceiling::core::coordinates::DeclinationError;

namespace
{
    constexpr auto declination =
        Declination::try_from(Angle::from_degrees(-23.5));
    static_assert(declination.has_value());

    constexpr auto invalid_declination =
        Declination::try_from(Angle::from_degrees(91.0));
    static_assert(!invalid_declination.has_value());
    static_assert(invalid_declination.error() ==
                  DeclinationError::OutOfRange);

    struct DeclinationCase
    {
        Angle value;
    };

    class ValidDeclinationTest
        : public ::testing::TestWithParam<DeclinationCase>
    {};

    class InvalidDeclinationOutOfRangeTest
        : public ::testing::TestWithParam<DeclinationCase>
    {};

    class InvalidDeclinationNonFiniteValuesTest
        : public ::testing::TestWithParam<DeclinationCase>
    {};

    void PrintTo(const DeclinationCase& tcase, std::ostream* os)
    {
        // how to print precise value of radiant to xth digit?
        *os << "value: " << tcase.value.to_degrees()
            << " deg (= " << tcase.value.to_radian() << ")";
    }
} // namespace

TEST_P(ValidDeclinationTest, TestsValidDeclination)
{
    const auto& tcase = GetParam();
    const auto dec = Declination::try_from(tcase.value);
    ASSERT_TRUE(dec.has_value());
    EXPECT_EQ(dec->get_angle().to_radian(), tcase.value.to_radian());
}

TEST_P(InvalidDeclinationNonFiniteValuesTest,
       TestsInvalidDeclinationNonFiniteValues)
{
    const auto& tcase = GetParam();
    const auto dec = Declination::try_from(tcase.value);

    ASSERT_FALSE(dec.has_value());
    EXPECT_EQ(dec.error(), DeclinationError::NonFinite);
}

TEST_P(InvalidDeclinationOutOfRangeTest, TestsInvalidDeclinationOutOfRange)
{
    const auto& tcase = GetParam();
    const auto dec = Declination::try_from(tcase.value);

    ASSERT_FALSE(dec.has_value());
    EXPECT_EQ(dec.error(), DeclinationError::OutOfRange);
}

INSTANTIATE_TEST_SUITE_P(
    ValidTests, ValidDeclinationTest,
    ::testing::Values(
        DeclinationCase{Angle::from_radian(0)},
        DeclinationCase{Angle::from_radian(-std::numbers::pi / 4)},
        DeclinationCase{Angle::from_radian(std::numbers::pi / 4)},
        DeclinationCase{Angle::from_radian(-std::numbers::pi * 3 / 7)},
        DeclinationCase{Angle::from_radian(std::numbers::pi * 3 / 7)},
        DeclinationCase{Angle::from_radian(-std::numbers::pi / 2)},
        DeclinationCase{Angle::from_radian(std::numbers::pi / 2)},
        DeclinationCase{
            Angle::from_radian(std::nextafter(std::numbers::pi / 2, 0))},
        DeclinationCase{
            Angle::from_radian(std::nextafter(-std::numbers::pi / 2, 0))},
        DeclinationCase{Angle::from_radian(std::nextafter(0, 1))},
        DeclinationCase{Angle::from_radian(std::nextafter(0, -1))}));

INSTANTIATE_TEST_SUITE_P(
    NonFiniteValuesTests, InvalidDeclinationNonFiniteValuesTest,
    ::testing::Values(
        DeclinationCase{
            Angle::from_radian(std::numeric_limits<double>::infinity())},
        DeclinationCase{
            Angle::from_radian(-std::numeric_limits<double>::infinity())},
        DeclinationCase{
            Angle::from_radian(std::numeric_limits<double>::quiet_NaN())}));

INSTANTIATE_TEST_SUITE_P(
    OutOfRange, InvalidDeclinationOutOfRangeTest,
    ::testing::Values(DeclinationCase{Angle::from_radian(std::numbers::pi)},
                      DeclinationCase{Angle::from_radian(-std::numbers::pi)},
                      DeclinationCase{Angle::from_radian(std::nextafter(
                          std::numbers::pi / 2, std::numbers::pi))},
                      DeclinationCase{Angle::from_radian(std::nextafter(
                          -std::numbers::pi / 2, -std::numbers::pi))}));
