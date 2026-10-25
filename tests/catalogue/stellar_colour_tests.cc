#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <mystarceiling/catalogue/stellar_colour.hh>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>

using msc::catalogue::colour_from_gaia_bp_rp;
using msc::catalogue::colour_from_hipparcos_b_v;
using msc::catalogue::colour_from_temperature_kelvin;
using msc::core::astronomy::LinearRgb8;

TEST(StellarColourTest, PacksThreeBytesWithHalfStepQuantisationBound)
{
    EXPECT_EQ(sizeof(LinearRgb8), 3);
    for (int index = 0; index <= 10000; ++index)
    {
        const auto value = index / 10000.0;
        const auto colour = LinearRgb8::try_from_linear({value, value, value});
        ASSERT_TRUE(colour);
        EXPECT_LE(std::abs(colour->red / 255.0 - value), 0.5 / 255.0 + 1e-15);
    }
}

TEST(StellarColourTest, RejectsInvalidLinearChannels)
{
    for (const auto value :
         {-0.01, 1.01, std::numeric_limits<double>::infinity(),
          std::numeric_limits<double>::quiet_NaN()})
        for (std::size_t channel = 0; channel < 3; ++channel)
        {
            std::array<double, 3> values{0.5, 0.5, 0.5};
            values[channel] = value;
            EXPECT_FALSE(LinearRgb8::try_from_linear(values));
        }
}

TEST(StellarColourTest, StoresLinearLightRatherThanEncodedSrgb)
{
    // Independent standard sRGB midpoint: encoded 0.5 decodes to 0.21404114.
    const auto midpoint = LinearRgb8::try_from_linear(
        {0.21404114048223255, 0.21404114048223255, 0.21404114048223255});
    ASSERT_TRUE(midpoint);
    EXPECT_EQ(*midpoint, (LinearRgb8{55, 55, 55}));
    EXPECT_EQ(*colour_from_temperature_kelvin(3000),
              (LinearRgb8{255, 112, 40}));
    EXPECT_EQ(*colour_from_temperature_kelvin(6500),
              (LinearRgb8{255, 253, 244}));
    EXPECT_EQ(*colour_from_temperature_kelvin(10000),
              (LinearRgb8{150, 179, 255}));
}

TEST(StellarColourTest, SourceSpecificIndicesProduceWarmAndCoolColours)
{
    const auto blue_gaia = colour_from_gaia_bp_rp(-0.2);
    const auto red_gaia = colour_from_gaia_bp_rp(2.0);
    const auto blue_hip = colour_from_hipparcos_b_v(-0.2);
    const auto red_hip = colour_from_hipparcos_b_v(1.8);
    ASSERT_TRUE(blue_gaia && red_gaia && blue_hip && red_hip);
    EXPECT_GT(blue_gaia->blue, blue_gaia->red);
    EXPECT_GT(blue_hip->blue, blue_hip->red);
    EXPECT_GT(red_gaia->red, red_gaia->blue);
    EXPECT_GT(red_hip->red, red_hip->blue);
}

TEST(StellarColourTest, RedExtensionJoinsContinuouslyAndRetainsVeryRedStars)
{
    EXPECT_EQ(*colour_from_gaia_bp_rp(1.5 - 1e-9),
              *colour_from_gaia_bp_rp(1.5 + 1e-9));
    EXPECT_NE(*colour_from_gaia_bp_rp(2), *colour_from_gaia_bp_rp(6));
    EXPECT_EQ(*colour_from_gaia_bp_rp(15), *colour_from_gaia_bp_rp(100));
}

TEST(StellarColourTest, FiniteExtremesHaveBoundedAppearance)
{
    const auto maximum = std::numeric_limits<double>::max();
    EXPECT_EQ(*colour_from_gaia_bp_rp(-maximum), *colour_from_gaia_bp_rp(-0.5));
    EXPECT_EQ(*colour_from_gaia_bp_rp(maximum), *colour_from_gaia_bp_rp(15));
    EXPECT_EQ(*colour_from_hipparcos_b_v(-maximum),
              *colour_from_hipparcos_b_v(-0.4));
    EXPECT_EQ(*colour_from_hipparcos_b_v(maximum),
              *colour_from_hipparcos_b_v(2));
    EXPECT_EQ(*colour_from_temperature_kelvin(1),
              *colour_from_temperature_kelvin(1000));
    EXPECT_EQ(*colour_from_temperature_kelvin(maximum),
              *colour_from_temperature_kelvin(40000));
}

TEST(StellarColourTest, RejectsNonfiniteSourceIndicesAndInvalidTemperature)
{
    for (const auto value : {std::numeric_limits<double>::infinity(),
                             -std::numeric_limits<double>::infinity(),
                             std::numeric_limits<double>::quiet_NaN()})
    {
        EXPECT_FALSE(colour_from_gaia_bp_rp(value));
        EXPECT_FALSE(colour_from_hipparcos_b_v(value));
        EXPECT_FALSE(colour_from_temperature_kelvin(value));
    }
    EXPECT_FALSE(colour_from_temperature_kelvin(0));
    EXPECT_FALSE(colour_from_temperature_kelvin(-1));
}
