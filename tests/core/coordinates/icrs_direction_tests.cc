
#include <gtest/gtest.h>

#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>

#include <limits>
#include <ostream>

using msc::core::coordinates::Declination;
using msc::core::coordinates::IcrsDirection;
using msc::core::coordinates::RightAscension;
using msc::core::maths::Angle;

namespace
{
    constexpr double floating_point_tolerance =
        64.0 * std::numeric_limits<double>::epsilon();

    struct CartesianCase
    {
        double right_ascension_hours;
        double declination_degrees;
        double expected_x;
        double expected_y;
        double expected_z;
    };

    class CartesianConversionTest
        : public ::testing::TestWithParam<CartesianCase>
    {};

    void PrintTo(const CartesianCase& test_case, std::ostream* output)
    {
        *output << "RA=" << test_case.right_ascension_hours
                << " h, dec=" << test_case.declination_degrees << " deg";
    }

    constexpr auto right_ascension =
        RightAscension::try_from(Angle::from_hours(5.5));
    constexpr auto declination =
        Declination::try_from(Angle::from_degrees(-23.5));
    static_assert(right_ascension.has_value());
    static_assert(declination.has_value());

    constexpr auto direction = IcrsDirection{*right_ascension, *declination};
    static_assert(direction.get_right_ascension().get_angle().to_radian() ==
                  right_ascension->get_angle().to_radian());
    static_assert(direction.get_declination().get_angle().to_radian() ==
                  declination->get_angle().to_radian());
} // namespace

TEST(IcrsDirectionTest, PreservesComponentsThroughConstAccessors)
{
    const auto right_ascension =
        RightAscension::try_from(Angle::from_hours(5.5));
    const auto declination = Declination::try_from(Angle::from_degrees(-23.75));

    ASSERT_TRUE(right_ascension.has_value());
    ASSERT_TRUE(declination.has_value());

    const auto direction = IcrsDirection(*right_ascension, *declination);

    EXPECT_DOUBLE_EQ(direction.get_right_ascension().get_angle().to_radian(),
                     right_ascension->get_angle().to_radian());
    EXPECT_DOUBLE_EQ(direction.get_declination().get_angle().to_radian(),
                     declination->get_angle().to_radian());
}

TEST_P(CartesianConversionTest, MapsIcrsDirectionToExpectedUnitVector)
{
    const auto& test_case = GetParam();
    const auto right_ascension = RightAscension::try_from(
        Angle::from_hours(test_case.right_ascension_hours));
    const auto declination = Declination::try_from(
        Angle::from_degrees(test_case.declination_degrees));

    ASSERT_TRUE(right_ascension.has_value());
    ASSERT_TRUE(declination.has_value());

    const auto vector =
        IcrsDirection{*right_ascension, *declination}.to_unit_vector();

    EXPECT_NEAR(vector.x(), test_case.expected_x, floating_point_tolerance);
    EXPECT_NEAR(vector.y(), test_case.expected_y, floating_point_tolerance);
    EXPECT_NEAR(vector.z(), test_case.expected_z, floating_point_tolerance);
}

TEST(IcrsDirectionTest, ProducesUnitLengthForNonAxisDirection)
{
    const auto right_ascension =
        RightAscension::try_from(Angle::from_hours(5.5));
    const auto declination = Declination::try_from(Angle::from_degrees(-23.75));

    ASSERT_TRUE(right_ascension.has_value());
    ASSERT_TRUE(declination.has_value());

    const auto vector =
        IcrsDirection{*right_ascension, *declination}.to_unit_vector();
    const auto squared_length = vector.x() * vector.x() +
        vector.y() * vector.y() + vector.z() * vector.z();

    EXPECT_NEAR(squared_length, 1.0, floating_point_tolerance);
}

INSTANTIATE_TEST_SUITE_P(
    PrincipalDirections, CartesianConversionTest,
    ::testing::Values(CartesianCase{0.0, 0.0, 1.0, 0.0, 0.0},
                      CartesianCase{6.0, 0.0, 0.0, 1.0, 0.0},
                      CartesianCase{12.0, 0.0, -1.0, 0.0, 0.0},
                      CartesianCase{18.0, 0.0, 0.0, -1.0, 0.0},
                      CartesianCase{7.0, 90.0, 0.0, 0.0, 1.0},
                      CartesianCase{7.0, -90.0, 0.0, 0.0, -1.0}));
