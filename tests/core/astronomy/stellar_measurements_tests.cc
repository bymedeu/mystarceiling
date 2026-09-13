#include <gtest/gtest.h>

#include <initializer_list>
#include <limits>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>

using msc::core::astronomy::ApparentMagnitude;
using msc::core::astronomy::ColourIndex;
using msc::core::astronomy::MeasurementError;
using msc::core::astronomy::Parallax;

TEST(StellarMeasurementsTest, RejectsNonFiniteMeasurements)
{
    for (const double value : {std::numeric_limits<double>::infinity(),
                               -std::numeric_limits<double>::infinity(),
                               std::numeric_limits<double>::quiet_NaN()})
    {
        const auto parallax = Parallax::try_from_milliarcseconds(value);
        const auto magnitude = ApparentMagnitude::try_from_gaia_g(value);
        const auto colour = ColourIndex::try_from_gaia_bp_minus_rp(value);
        ASSERT_FALSE(parallax);
        ASSERT_FALSE(magnitude);
        ASSERT_FALSE(colour);
        EXPECT_EQ(parallax.error(), MeasurementError::NonFinite);
        EXPECT_EQ(magnitude.error(), MeasurementError::NonFinite);
        EXPECT_EQ(colour.error(), MeasurementError::NonFinite);
    }
}

TEST(StellarMeasurementsTest, AcceptsSignedAndZeroMeasurements)
{
    for (const double value : {-1.0, 0.0, 1.0})
    {
        EXPECT_TRUE(Parallax::try_from_milliarcseconds(value));
        EXPECT_TRUE(ApparentMagnitude::try_from_gaia_g(value));
        EXPECT_TRUE(ColourIndex::try_from_gaia_bp_minus_rp(value));
    }
}
