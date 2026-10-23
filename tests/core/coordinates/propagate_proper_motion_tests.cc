#include <gtest/gtest.h>

#include <cmath>
#include <initializer_list>
#include <limits>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/propagate_proper_motion.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <numbers>

namespace
{
    using msc::core::coordinates::Declination;
    using msc::core::coordinates::IcrsDirection;
    using msc::core::coordinates::propagate_proper_motion;
    using msc::core::coordinates::ProperMotion;
    using msc::core::coordinates::ProperMotionComponents;
    using msc::core::coordinates::ProperMotionPropagationError;
    using msc::core::coordinates::RightAscension;
    using msc::core::maths::Angle;
    using msc::core::time::ReferenceEpoch;
    constexpr double pi = std::numbers::pi;
    constexpr double mas_to_rad = pi / (180.0 * 3600000.0);
    // A few tens of double roundoff units, in radians, for trig and
    // normalization.
    constexpr double tolerance = 8e-15;

    IcrsDirection direction(double ra, double dec)
    {
        return {*RightAscension::try_from(Angle::from_radian(ra)),
                *Declination::try_from(Angle::from_radian(dec))};
    }
    ReferenceEpoch epoch(double year)
    {
        return *ReferenceEpoch::try_from_julian_year(year);
    }
    ProperMotionComponents motion(double east, double north)
    {
        return {.right_ascension_cos_declination =
                    *ProperMotion::try_from_milliarcseconds_per_year(east),
                .declination =
                    *ProperMotion::try_from_milliarcseconds_per_year(north)};
    }
    double ra(IcrsDirection value)
    {
        return value.get_right_ascension().get_angle().to_radian();
    }
    double dec(IcrsDirection value)
    {
        return value.get_declination().get_angle().to_radian();
    }

    TEST(ProperMotionPropagationTest, ZeroIntervalPreservesInputExactly)
    {
        for (double latitude : {0.3, pi / 2, -pi / 2})
        {
            const auto input = direction(1.7, latitude);
            const auto result = propagate_proper_motion(
                input, motion(std::numeric_limits<double>::max(), 1),
                epoch(2016), epoch(2016));
            ASSERT_TRUE(result);
            EXPECT_EQ(ra(*result), ra(input));
            EXPECT_EQ(dec(*result), dec(input));
        }
    }

    TEST(ProperMotionPropagationTest, ZeroMotionPreservesInputIncludingPoles)
    {
        for (const auto input : {direction(4.2, -0.7), direction(2.0, pi / 2),
                                 direction(2.0, -pi / 2)})
        {
            const auto result = propagate_proper_motion(
                input, motion(0, 0), epoch(2016), epoch(2100));
            ASSERT_TRUE(result);
            EXPECT_EQ(ra(*result), ra(input));
            EXPECT_EQ(dec(*result), dec(input));
        }
    }

    TEST(ProperMotionPropagationTest, ForwardAndBackwardAnalyticMotion)
    {
        for (double elapsed_years : {-20.0, 20.0})
        {
            const auto east = propagate_proper_motion(
                direction(1, 0), motion(1'000'000, 0), epoch(2000),
                epoch(2000 + elapsed_years));
            const auto north = propagate_proper_motion(
                direction(1, 0), motion(0, 1'000'000), epoch(2000),
                epoch(2000 + elapsed_years));
            ASSERT_TRUE(east && north);
            EXPECT_NEAR(ra(*east),
                        1 + std::atan(elapsed_years * 1'000'000 * mas_to_rad),
                        tolerance);
            EXPECT_EQ(dec(*east), 0);
            EXPECT_NEAR(ra(*north), 1, tolerance);
            EXPECT_NEAR(dec(*north),
                        std::atan(elapsed_years * 1'000'000 * mas_to_rad),
                        tolerance);
        }
    }

    TEST(ProperMotionPropagationTest,
         HighDeclinationUsesRaStarWithoutAnotherCosine)
    {
        const double latitude = 85 * pi / 180;
        const double shift = 100'000 * mas_to_rad * 10;
        const auto result =
            propagate_proper_motion(direction(0, latitude), motion(100'000, 0),
                                    epoch(2000), epoch(2010));
        ASSERT_TRUE(result);
        EXPECT_NEAR(ra(*result), std::atan2(shift, std::cos(latitude)),
                    tolerance);
        EXPECT_NEAR(dec(*result),
                    std::atan2(std::sin(latitude),
                               std::hypot(std::cos(latitude), shift)),
                    tolerance);
    }

    TEST(ProperMotionPropagationTest,
         BothPolesHaveDeterministicOutputAndTangentBasis)
    {
        for (double sign : {-1.0, 1.0})
        {
            const auto input = direction(0, sign * pi / 2);
            const auto east = propagate_proper_motion(
                input, motion(1'000'000, 0), epoch(2000), epoch(2001));
            const auto north = propagate_proper_motion(
                input, motion(0, 1'000'000), epoch(2000), epoch(2001));
            ASSERT_TRUE(east && north);
            EXPECT_NEAR(ra(*east), pi / 2, tolerance);
            EXPECT_NEAR(ra(*north), sign > 0 ? pi : 0, tolerance);
            EXPECT_NEAR(dec(*east),
                        sign * (pi / 2 - std::atan(1'000'000 * mas_to_rad)),
                        tolerance);
            EXPECT_NEAR(dec(*north), dec(*east), tolerance);
        }
    }

    TEST(ProperMotionPropagationTest,
         RecoversDeclinationNearPoleWithoutAsinRounding)
    {
        const double latitude = pi / 2 - 1e-10;
        const auto result =
            propagate_proper_motion(direction(0.7, latitude), motion(0, 1e-12),
                                    epoch(2000), epoch(2001));
        ASSERT_TRUE(result);
        EXPECT_NEAR(dec(*result), latitude,
                    2 * std::numeric_limits<double>::epsilon());
        EXPECT_LT(dec(*result), pi / 2);
    }

    TEST(ProperMotionPropagationTest, CrossesRaSeamInBothDirections)
    {
        for (double sign : {-1.0, 1.0})
        {
            const double start = sign > 0 ? 2 * pi - 0.001 : 0.001;
            const auto result = propagate_proper_motion(
                direction(start, 0), motion(sign * 1'000'000, 0), epoch(2000),
                epoch(2001));
            ASSERT_TRUE(result);
            double expected = start + std::atan(sign * 1'000'000 * mas_to_rad);
            if (expected < 0)
                expected += 2 * pi;
            if (expected >= 2 * pi)
                expected -= 2 * pi;
            EXPECT_NEAR(ra(*result), expected, tolerance);
            EXPECT_GE(ra(*result), 0);
            EXPECT_LT(ra(*result), 2 * pi);
        }
        const auto rounded = propagate_proper_motion(
            direction(0, 0), motion(-1e-20, 0), epoch(2000), epoch(2001));
        ASSERT_TRUE(rounded);
        EXPECT_EQ(ra(*rounded), 0);
    }

    TEST(ProperMotionPropagationTest,
         DistinguishesNonFiniteEpochAndDisplacement)
    {
        const double maximum = std::numeric_limits<double>::max();
        const auto interval = propagate_proper_motion(
            direction(0, 0), motion(0, 0), epoch(-maximum), epoch(maximum));
        const auto displacement = propagate_proper_motion(
            direction(0, 0), motion(maximum, 0), epoch(0), epoch(maximum));
        const auto components = propagate_proper_motion(
            direction(pi / 4, pi / 2), motion(maximum, maximum), epoch(0),
            epoch(2e8));

        ASSERT_FALSE(interval);
        ASSERT_FALSE(displacement);
        ASSERT_FALSE(components);
        EXPECT_EQ(interval.error(),
                  ProperMotionPropagationError::NonFiniteEpochDifference);
        EXPECT_EQ(displacement.error(),
                  ProperMotionPropagationError::NonFiniteDisplacement);
        EXPECT_EQ(components.error(),
                  ProperMotionPropagationError::NonFiniteDisplacement);
    }

    TEST(ProperMotionPropagationTest, NormalizesHugeFiniteDisplacements)
    {
        const auto result = propagate_proper_motion(
            direction(0, 0), motion(1e300, 1e300), epoch(0), epoch(1));
        ASSERT_TRUE(result);
        EXPECT_NEAR(ra(*result), pi / 2, tolerance);
        EXPECT_NEAR(dec(*result), pi / 4, tolerance);
    }

    TEST(ProperMotionPropagationTest, MatchesIndependentSphericalCalculation)
    {
        // The fixed values were generated from an independent spherical
        // rearrangement of the selected tangent-vector model. This test does
        // not call the production Cartesian implementation or claim full ERFA
        // parity.
        const auto result = propagate_proper_motion(
            direction(1.234, 0.789),
            motion(1e-5 * std::cos(0.789) / mas_to_rad, -2e-5 / mas_to_rad),
            epoch(2000), epoch(2025));
        ASSERT_TRUE(result);
        EXPECT_NEAR(ra(*result), 1.2342498741544565, tolerance);
        EXPECT_NEAR(dec(*result), 0.78849998443269365, tolerance);
    }
} // namespace
