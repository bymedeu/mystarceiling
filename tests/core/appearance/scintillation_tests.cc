#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <mystarceiling/core/appearance/scintillation.hh>

using msc::core::appearance::AnimationClock;
using msc::core::appearance::AnimationKey;
using msc::core::appearance::hash_noise;
using msc::core::appearance::sample_twinkle;
using msc::core::appearance::scintillation_frame;
using msc::core::appearance::TwinkleSettings;
using msc::core::appearance::valid_twinkle_settings;

namespace
{
    // Recover the averaged linear deviation for exposure/horizon tests.
    // The final bounded visual response is intentionally nonlinear.
    double linear_deviation(double gain)
    {
        return 0.9 * std::atanh((gain - 1) / 0.9) / 10;
    }
} // namespace


TEST(ScintillationTest, HashMatchesIndependentUnsignedReferenceVectors)
{
    struct Reference
    {
        AnimationKey key;
        std::uint32_t counter{};
        std::uint32_t expected{};
    };
    // Generated independently with Python integers and explicit uint32 masks.
    for (const auto& reference :
         {Reference{{0, 0}, 0, 0}, Reference{{1, 0}, 719, 0xb127cb57U},
          Reference{{0, 1}, 0, 0xcafeba47U},
          Reference{{0x12345678U, 0x9abcdef0U}, 0xffffffffU, 0x447cc112U},
          Reference{{0xffffffffU, 0xffffffffU}, 1234, 0x048ca61dU}})
        EXPECT_EQ(hash_noise(reference.key, reference.counter),
                  reference.expected);
}

TEST(ScintillationTest, HighWordAndSourceSaltRemainPartOfIdentity)
{
    const auto first = hash_noise({1234, 0}, 719);
    const auto high_word = hash_noise({1234, 1}, 719);
    const auto source_salt = hash_noise({1234, 0x80000000U}, 719);
    EXPECT_NE(first, high_word);
    EXPECT_NE(first, source_salt);
    EXPECT_NE(high_word, source_salt);
}

TEST(ScintillationTest, SamplesAreDeterministicAndIndependentOfIterationOrder)
{
    const auto frame = scintillation_frame(17.125, 1.0 / 60);
    const std::array keys{AnimationKey{1, 0}, AnimationKey{1, 1},
                          AnimationKey{5, 0x80000000U}, AnimationKey{9, 3}};
    std::array<std::array<double, 3>, keys.size()> expected{};
    for (std::size_t index = 0; index < keys.size(); ++index)
        expected[index] = sample_twinkle(keys[index], frame, 0.3);
    for (std::size_t index = keys.size(); index-- > 0;)
        EXPECT_EQ(sample_twinkle(keys[index], frame, 0.3), expected[index]);
    EXPECT_NE(expected[0], expected[1]);
}

TEST(ScintillationTest, SpectralVarianceIsNormalizedWithoutSlowBandDominance)
{
    double variance{}, slow{}, fast{};
    for (const auto& band : scintillation_frame(10, 0))
    {
        const double power = double(band[1]) * band[1] / 2;
        variance += power;
        if (band[2] < 3)
            slow += power;
        if (band[2] > 8)
            fast += power;
    }
    // Float uniform amplitudes retain the normalized ensemble variance.
    EXPECT_NEAR(variance, 1, 1e-7);
    EXPECT_LT(slow / variance, 0.1);
    EXPECT_GT(fast / variance, 0.8);
}

TEST(ScintillationTest, ExposureReducesVarianceWithoutRenormalizingIt)
{
    std::array<double, 4> variance{};
    const std::array exposures{0.0, 1.0 / 60, 0.1, 0.5};
    for (std::size_t i = 0; i < exposures.size(); ++i)
        for (const auto& band : scintillation_frame(10, exposures[i]))
            variance[i] += double(band[1]) * band[1] / 2;
    EXPECT_GT(variance[0], variance[1]);
    EXPECT_GT(variance[1], variance[2]);
    EXPECT_GT(variance[2], variance[3]);
    EXPECT_LT(variance[3], 0.02 * variance[0]);
}

TEST(ScintillationTest,
     AnalyticExposureMatchesNumericalIntegrationBeforeVisualResponse)
{
    const AnimationKey key{81, 9};
    constexpr double end = 1.25, exposure = 0.031;
    constexpr int samples = 8192;
    const auto integrated =
        sample_twinkle(key, scintillation_frame(end, exposure), 0.2);
    std::array<double, 3> numerical{};
    for (int i = 0; i < samples; ++i)
    {
        const auto t = end - exposure + (i + 0.5) * exposure / samples;
        const auto instant =
            sample_twinkle(key, scintillation_frame(t, 0), 0.2);
        for (std::size_t c = 0; c < numerical.size(); ++c)
            numerical[c] += linear_deviation(instant[c]) / samples;
    }
    // Midpoint quadrature plus float phase/amplitude upload rounding.
    for (std::size_t c = 0; c < numerical.size(); ++c)
        EXPECT_NEAR(linear_deviation(integrated[c]), numerical[c], 2e-6);
}

TEST(ScintillationTest, ExposurePartitionsPreserveTheLinearSignalAverage)
{
    const AnimationKey key{819, 1};
    const auto whole = sample_twinkle(key, scintillation_frame(2, 0.05), 0);
    const auto left = sample_twinkle(key, scintillation_frame(1.975, 0.025), 0);
    const auto right = sample_twinkle(key, scintillation_frame(2, 0.025), 0);
    for (std::size_t c = 0; c < whole.size(); ++c)
        EXPECT_NEAR(
            linear_deviation(whole[c]),
            (linear_deviation(left[c]) + linear_deviation(right[c])) / 2, 2e-7);
}

TEST(ScintillationTest, StartupExposureIsClippedToAnimationOrigin)
{
    EXPECT_EQ(scintillation_frame(0.001, 1.0 / 60),
              scintillation_frame(0.001, 0.001));
    EXPECT_EQ(scintillation_frame(0, 1.0 / 60), scintillation_frame(0, 0));
    const auto instant = sample_twinkle({8, 3}, scintillation_frame(10, 0), 0);
    const auto tiny = sample_twinkle({8, 3}, scintillation_frame(10, 1e-12), 0);
    for (std::size_t c = 0; c < instant.size(); ++c)
        EXPECT_NEAR(instant[c], tiny[c], 1e-7);
}

TEST(ScintillationTest, FinitePositiveMultipliersRespectTheBoundedResponse)
{
    for (double seconds : {0.0, 0.125, 7.75, 4096.5, 1e12})
    {
        const auto frame = scintillation_frame(seconds, 1.0 / 60);
        for (std::uint32_t id = 0; id < 64; ++id)
            for (double elevation : {0.0, 0.5, 1.0})
                for (double multiplier :
                     sample_twinkle({id, id ^ 0x80000000U}, frame, elevation))
                {
                    EXPECT_TRUE(std::isfinite(multiplier));
                    EXPECT_GT(multiplier, 0);
                    EXPECT_GE(multiplier, 1 - 0.9);
                    EXPECT_LE(multiplier, 1 + 0.9);
                }
    }
}

TEST(ScintillationTest, CommonSignalRetainsHorizonDependence)
{
    TwinkleSettings settings;
    settings.colour_delay = 0;
    const auto frame = scintillation_frame(1.25, 1.0 / 60);
    const auto horizon = sample_twinkle({81, 9}, frame, 0, settings);
    const auto zenith = sample_twinkle({81, 9}, frame, 1, settings);
    EXPECT_NE(horizon[0], 1);
    for (std::size_t c = 0; c < horizon.size(); ++c)
        EXPECT_NEAR(linear_deviation(zenith[c]),
                    settings.zenith_fraction * linear_deviation(horizon[c]),
                    1e-15);
    settings.zenith_fraction = 0;
    EXPECT_EQ(sample_twinkle({81, 9}, frame, 1, settings),
              (std::array<double, 3>{1, 1, 1}));
}

TEST(ScintillationTest, ColourChannelsShareTheSignalWithDifferentialDelays)
{
    const auto frame = scintillation_frame(1.25, 1.0 / 60);
    const auto zenith = sample_twinkle({81, 9}, frame, 1);
    EXPECT_EQ(zenith[0], zenith[1]);
    EXPECT_EQ(zenith[1], zenith[2]);
    const auto horizon = sample_twinkle({81, 9}, frame, 0);
    EXPECT_NE(horizon[0], horizon[1]);
    EXPECT_NE(horizon[1], horizon[2]);
    TwinkleSettings settings;
    settings.colour_delay = 0;
    const auto common = sample_twinkle({81, 9}, frame, 0, settings);
    EXPECT_EQ(common[0], common[1]);
    EXPECT_EQ(common[1], common[2]);
    EXPECT_EQ(common[1], horizon[1]);
}

TEST(ScintillationTest,
     ZeroStrengthPreservesChannelsAndInvalidSettingsAreRejected)
{
    TwinkleSettings settings;
    settings.strength = 0;
    EXPECT_EQ(
        sample_twinkle({8, 6}, scintillation_frame(151.875, 0.02), 0, settings),
        (std::array<double, 3>{1, 1, 1}));
    ASSERT_TRUE(valid_twinkle_settings({}));
    for (const auto member :
         {&TwinkleSettings::strength, &TwinkleSettings::zenith_fraction,
          &TwinkleSettings::colour_delay})
        for (const double invalid :
             {-0.1, 1.01, std::numeric_limits<double>::infinity(),
              std::numeric_limits<double>::quiet_NaN()})
        {
            settings = {};
            settings.*member = invalid;
            EXPECT_FALSE(valid_twinkle_settings(settings));
        }
    settings = {};
    settings.strength = 1; // The bounded response guarantees positive light.
    EXPECT_TRUE(valid_twinkle_settings(settings));
}

TEST(AnimationClockTest, PictureModeFreezesTimeAndExposureWithoutRestarting)
{
    AnimationClock clock;
    clock.advance(0.02);
    const auto before = scintillation_frame(clock.seconds(), clock.exposure());
    clock.set_picture_mode(true);
    clock.advance(100);
    EXPECT_EQ(clock.seconds(), 0.02);
    EXPECT_EQ(clock.exposure(), 0.02);
    EXPECT_EQ(scintillation_frame(clock.seconds(), clock.exposure()), before);
    clock.set_picture_mode(false);
    clock.advance(0.01);
    EXPECT_EQ(clock.seconds(), 0.03);
    EXPECT_EQ(clock.exposure(), 0.01);
}

TEST(AnimationClockTest, TimeDependsOnElapsedTimeAndExposureOnTheLastFrame)
{
    AnimationClock single, partitioned;
    single.advance(2);
    for (int frame = 0; frame < 8; ++frame)
        partitioned.advance(0.25);
    EXPECT_EQ(single.seconds(), partitioned.seconds());
    EXPECT_EQ(single.exposure(), 2);
    EXPECT_EQ(partitioned.exposure(), 0.25);
}

TEST(AnimationClockTest, InvalidAndOverflowingUpdatesPreserveBothValues)
{
    AnimationClock clock;
    clock.advance(2);
    for (const double invalid :
         {-1.0, 0.0, std::numeric_limits<double>::infinity(),
          std::numeric_limits<double>::quiet_NaN()})
    {
        clock.advance(invalid);
        EXPECT_EQ(clock.seconds(), 2);
        EXPECT_EQ(clock.exposure(), 2);
    }
    clock.advance(std::numeric_limits<double>::max());
    const auto before = clock.seconds();
    clock.advance(std::numeric_limits<double>::max());
    EXPECT_EQ(clock.seconds(), before);
    EXPECT_EQ(clock.exposure(), before);
}

TEST(AnimationClockTest, LargeTimeStillAdvancesAndProducesFiniteReducedPhases)
{
    AnimationClock clock;
    clock.advance(1e12);
    const auto before = scintillation_frame(clock.seconds(), 0.01);
    clock.advance(0.125);
    const auto after = scintillation_frame(clock.seconds(), clock.exposure());
    EXPECT_EQ(clock.seconds(), 1e12 + 0.125);
    EXPECT_NE(before, after);
    for (const auto& band : after)
    {
        EXPECT_GE(band[0], 0);
        EXPECT_LE(band[0], 1); // Float rounding may reach one complete cycle.
        EXPECT_TRUE(std::isfinite(band[1]));
        EXPECT_TRUE(std::isfinite(band[2]));
    }
}

TEST(ScintillationTest, ApprovedBaselineUsesTheSelectedBoundedResponse)
{
    msc::core::appearance::ScintillationFrame frame{};
    constexpr AnimationKey key{0, 0};
    const auto offset = double(hash_noise(key, 719) >> 8U) / 16777216.0;
    frame[0] = {static_cast<float>(-offset), 0.25F, 0};
    const auto gain = sample_twinkle(key, frame, 0);
    // A known 0.03 deviation maps to the approved baseline response.
    for (const auto value : gain)
        EXPECT_NEAR(value, 1 + 0.9 * std::tanh(1.0 / 3), 1e-12);
    frame[0][1] = 1000;
    const auto saturated = sample_twinkle(key, frame, 0);
    for (const auto value : saturated)
        EXPECT_DOUBLE_EQ(value, 1.9);
}
