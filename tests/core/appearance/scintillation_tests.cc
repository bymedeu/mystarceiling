#include <gtest/gtest.h>

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
using msc::core::appearance::noise_clock;
using msc::core::appearance::NoiseClock;
using msc::core::appearance::sample_twinkle;
using msc::core::appearance::smooth_noise;
using msc::core::appearance::TwinkleSettings;
using msc::core::appearance::valid_twinkle_settings;

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

TEST(ScintillationTest, NoiseClockRetainsFractionAcrossLargeIntervalsAndWrap)
{
    constexpr double period = 4294967296.0;
    const auto ordinary = noise_clock(4096.25, 2);
    EXPECT_EQ(ordinary.interval, 8192U);
    EXPECT_EQ(ordinary.fraction, 0.5F);
    const auto large = noise_clock(1e12 + 0.125, 2);
    EXPECT_EQ(large.interval, static_cast<std::uint32_t>(2000000000000ULL));
    EXPECT_EQ(large.fraction, 0.25F);
    const auto before = noise_clock(period - 0.25, 1);
    EXPECT_EQ(before.interval, std::numeric_limits<std::uint32_t>::max());
    EXPECT_EQ(before.fraction, 0.75F);
    const auto after = noise_clock(period + 0.25, 1);
    EXPECT_EQ(after.interval, 0U);
    EXPECT_EQ(after.fraction, 0.25F);
}

TEST(ScintillationTest, NoiseClockKeepsFractionHalfOpenAfterFloatRounding)
{
    const auto clock = noise_clock(std::nextafter(1.0, 0.0), 1);
    EXPECT_EQ(clock.interval, 1U);
    EXPECT_EQ(clock.fraction, 0.0F);
    const auto enormous = noise_clock(std::numeric_limits<double>::max(),
                                      std::numeric_limits<double>::max());
    EXPECT_TRUE(std::isfinite(enormous.fraction));
    EXPECT_GE(enormous.fraction, 0);
    EXPECT_LT(enormous.fraction, 1);
    const auto stopped = noise_clock(std::numeric_limits<double>::max(), 0);
    EXPECT_EQ(stopped.interval, 0U);
    EXPECT_EQ(stopped.fraction, 0.0F);
}

TEST(ScintillationTest, SmoothNoiseIsContinuousAtItsPerStarIntervalBoundary)
{
    const AnimationKey key{0x12345678U, 0x9abcdef0U};
    const auto phase = static_cast<double>(hash_noise(key, 719)) / 4294967295.0;
    const double boundary = 12.0 - phase;
    const auto at = smooth_noise(key, noise_clock(boundary, 1));
    const auto expected =
        2 * static_cast<double>(hash_noise(key, 12)) / 4294967295.0 - 1;
    // Smoothstep has zero slope at either endpoint; float clock rounding
    // contributes much less than 1e-10 for a 1e-6 neighbourhood here.
    EXPECT_NEAR(at, expected, 1e-12);
    EXPECT_NEAR(smooth_noise(key, noise_clock(boundary - 1e-6, 1)), expected,
                1e-10);
    EXPECT_NEAR(smooth_noise(key, noise_clock(boundary + 1e-6, 1)), expected,
                1e-10);
}

TEST(ScintillationTest, SmoothNoiseRemainsContinuousAcrossUint32Wrap)
{
    const AnimationKey key{7, 19};
    const auto last =
        smooth_noise(key,
                     NoiseClock{std::numeric_limits<std::uint32_t>::max(),
                                std::nextafter(1.0F, 0.0F)});
    const auto first = smooth_noise(key, NoiseClock{0, 0});
    // The derivative of remapped smoothstep noise is bounded by 3. The
    // fractional gap is 2^-24, so 3 * 2^-24 bounds the difference.
    EXPECT_NEAR(last, first, 3 * std::ldexp(1.0, -24));
}

TEST(ScintillationTest, SamplesAreDeterministicAndIndependentOfIterationOrder)
{
    const std::array keys{AnimationKey{1, 0}, AnimationKey{1, 1},
                          AnimationKey{5, 0x80000000U}, AnimationKey{9, 3}};
    std::array<std::array<double, 3>, keys.size()> expected{};
    for (std::size_t index = 0; index < keys.size(); ++index)
        expected[index] = sample_twinkle(keys[index], 17.125, 0.3, {});
    for (std::size_t index = keys.size(); index-- > 0;)
        EXPECT_EQ(sample_twinkle(keys[index], 17.125, 0.3, {}),
                  expected[index]);
    EXPECT_NE(expected[0], expected[1]);
}

TEST(ScintillationTest, FinitePositiveMultipliersRespectDerivedGlobalBounds)
{
    TwinkleSettings settings;
    settings.strength = 0.95;
    settings.colour_variation = 0.95;
    const auto minimum =
        (1 - settings.strength) * (1 - settings.colour_variation);
    const auto maximum =
        (1 + settings.strength) * (1 + settings.colour_variation);
    for (std::uint32_t identity = 0; identity < 32; ++identity)
        for (const double seconds : {0.0, 0.125, 7.75, 4096.5, 1e12})
            for (const double elevation : {0.0, 0.5, 1.0})
            {
                const auto key = AnimationKey{identity, identity ^ 0x80000000U};
                for (const auto multiplier :
                     sample_twinkle(key, seconds, elevation, settings))
                {
                    EXPECT_TRUE(std::isfinite(multiplier));
                    EXPECT_GT(multiplier, 0);
                    EXPECT_GE(multiplier, minimum);
                    EXPECT_LE(multiplier, maximum);
                }
            }
}

TEST(ScintillationTest, HorizonAndZenithHaveTheDeclaredAmplitudeRatio)
{
    TwinkleSettings settings;
    settings.colour_variation = 0;
    const auto horizon = sample_twinkle({81, 9}, 1.25, 0, settings);
    const auto zenith = sample_twinkle({81, 9}, 1.25, 1, settings);
    EXPECT_NE(horizon[0], 1);
    for (std::size_t channel = 0; channel < 3; ++channel)
        EXPECT_NEAR(zenith[channel] - 1,
                    settings.zenith_fraction * (horizon[channel] - 1), 3e-16);
    settings.zenith_fraction = 0;
    EXPECT_EQ(sample_twinkle({81, 9}, 1.25, 1, settings),
              (std::array<double, 3>{1, 1, 1}));
}

TEST(ScintillationTest, ZeroModulationPreservesAllChannelsExactly)
{
    TwinkleSettings settings;
    settings.strength = 0;
    settings.colour_variation = 0;
    EXPECT_EQ(sample_twinkle({8, 6}, 151.875, 0, settings),
              (std::array<double, 3>{1, 1, 1}));
}

TEST(ScintillationTest, InvalidSettingsAreRejectedBeforeSampling)
{
    ASSERT_TRUE(valid_twinkle_settings({}));
    for (const auto member :
         {&TwinkleSettings::strength, &TwinkleSettings::colour_variation,
          &TwinkleSettings::zenith_fraction, &TwinkleSettings::slow_weight})
        for (const auto invalid :
             {-0.1, 1.01, std::numeric_limits<double>::infinity(),
              std::numeric_limits<double>::quiet_NaN()})
        {
            TwinkleSettings settings;
            settings.*member = invalid;
            EXPECT_FALSE(valid_twinkle_settings(settings));
        }
    for (const auto member :
         {&TwinkleSettings::slow_rate, &TwinkleSettings::fast_rate})
        for (const auto invalid :
             {0.0, -1.0, std::numeric_limits<double>::infinity(),
              std::numeric_limits<double>::quiet_NaN()})
        {
            TwinkleSettings settings;
            settings.*member = invalid;
            EXPECT_FALSE(valid_twinkle_settings(settings));
        }
}

TEST(ScintillationTest, StrictlyPositiveOutputExcludesUnitModulationStrength)
{
    for (const auto member :
         {&TwinkleSettings::strength, &TwinkleSettings::colour_variation})
    {
        TwinkleSettings settings;
        settings.*member = 1;
        EXPECT_FALSE(valid_twinkle_settings(settings));
    }
    TwinkleSettings settings;
    settings.zenith_fraction = 1;
    settings.slow_weight = 0;
    EXPECT_TRUE(valid_twinkle_settings(settings));
}

TEST(AnimationClockTest, PictureModeFreezesWithoutAccumulatingOrRestarting)
{
    AnimationClock clock;
    clock.advance(0.5, 2);
    const auto before = sample_twinkle({5, 2}, clock.seconds(), 0.4, {});
    clock.set_picture_mode(true);
    clock.advance(100, 2);
    EXPECT_EQ(clock.seconds(), 1);
    EXPECT_EQ(sample_twinkle({5, 2}, clock.seconds(), 0.4, {}), before);
    clock.set_picture_mode(false);
    clock.advance(0.25, 2);
    EXPECT_EQ(clock.seconds(), 1.5);
}

TEST(AnimationClockTest, TimeDependsOnElapsedTimeRatherThanFrameCount)
{
    AnimationClock single;
    AnimationClock partitioned;
    single.advance(2, 0.5);
    for (int frame = 0; frame < 8; ++frame)
        partitioned.advance(0.25, 0.5);
    EXPECT_EQ(single.seconds(), partitioned.seconds());
    EXPECT_EQ(sample_twinkle({5, 2}, single.seconds(), 0.4, {}),
              sample_twinkle({5, 2}, partitioned.seconds(), 0.4, {}));
}

TEST(AnimationClockTest, InvalidUpdatesAndZeroSpeedLeaveClockUnchanged)
{
    AnimationClock clock;
    clock.advance(1, 2);
    for (const auto invalid : {-1.0, std::numeric_limits<double>::infinity(),
                               std::numeric_limits<double>::quiet_NaN()})
    {
        clock.advance(invalid, 1);
        clock.advance(1, invalid);
        EXPECT_EQ(clock.seconds(), 2);
    }
    clock.advance(1, 0);
    clock.advance(std::numeric_limits<double>::max(), 2);
    EXPECT_EQ(clock.seconds(), 2);
}

TEST(AnimationClockTest, LargeElapsedTimeKeepsItsOriginAndFraction)
{
    AnimationClock clock;
    clock.advance(1e12, 1);
    clock.advance(0.125, 1);
    EXPECT_EQ(clock.seconds(), 1e12 + 0.125);
    const auto sample = noise_clock(clock.seconds(), 2);
    EXPECT_EQ(sample.interval, static_cast<std::uint32_t>(2000000000000ULL));
    EXPECT_EQ(sample.fraction, 0.25F);
}
