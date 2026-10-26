#include <mystarceiling/core/appearance/scintillation.hh>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace msc::core::appearance
{
    namespace
    {
        constexpr double interval_period = 4294967296.0;
        constexpr double hash_maximum = 4294967295.0;

        std::uint32_t mixed(std::uint32_t value) noexcept
        {
            value = (value ^ (value >> 16U)) * 0x21f0aaadU;
            value = (value ^ (value >> 15U)) * 0x735a2d97U;
            return value ^ (value >> 15U);
        }

        double unit_noise(AnimationKey key, std::uint32_t counter) noexcept
        {
            return static_cast<double>(hash_noise(key, counter)) / hash_maximum;
        }

        bool nonnegative_finite(double value) noexcept
        {
            return std::isfinite(value) && value >= 0;
        }

        bool unit_interval(double value) noexcept
        {
            return nonnegative_finite(value) && value <= 1;
        }
        bool open_unit_interval(double value) noexcept
        {
            return nonnegative_finite(value) && value < 1;
        }

        bool positive_finite(double value) noexcept
        {
            return std::isfinite(value) && value > 0;
        }
    } // namespace

    bool valid_twinkle_settings(const TwinkleSettings& settings) noexcept
    {
        return open_unit_interval(settings.strength) &&
            open_unit_interval(settings.colour_variation) &&
            unit_interval(settings.zenith_fraction) &&
            unit_interval(settings.slow_weight) &&
            positive_finite(settings.slow_rate) &&
            positive_finite(settings.fast_rate);
    }

    NoiseClock noise_clock(double seconds, double rate) noexcept
    {
        auto ticks = seconds * rate;
        if (!std::isfinite(ticks))
            ticks = std::fmod(seconds, interval_period / rate) * rate;
        ticks = std::fmod(ticks, interval_period);
        const auto whole = std::floor(ticks);
        auto interval = static_cast<std::uint32_t>(whole);
        auto fraction = static_cast<float>(ticks - whole);
        // Rounding the fraction to float can reach the next interval.
        if (fraction == 1.0F)
        {
            ++interval;
            fraction = 0;
        }
        return {interval, fraction};
    }

    std::uint32_t hash_noise(AnimationKey key, std::uint32_t counter) noexcept
    {
        return mixed(key.low ^ mixed(key.high) ^ (counter * 0x9e3779b9U));
    }

    double smooth_noise(AnimationKey key, NoiseClock clock) noexcept
    {
        auto fraction =
            static_cast<double>(clock.fraction) + unit_noise(key, 719);
        if (fraction >= 1)
        {
            ++clock.interval;
            fraction -= 1;
        }
        const auto blend = fraction * fraction * (3 - 2 * fraction);
        const auto first = unit_noise(key, clock.interval);
        const auto second = unit_noise(key, clock.interval + 1U);
        return 2 * (first + blend * (second - first)) - 1;
    }

    std::array<double, 3>
    sample_twinkle(AnimationKey key, double seconds, double sine_elevation,
                   const TwinkleSettings& settings) noexcept
    {
        const auto horizon = 1 - std::clamp(sine_elevation, 0.0, 1.0);
        const auto factor = settings.zenith_fraction +
            (1 - settings.zenith_fraction) * horizon * horizon;
        const auto slow =
            smooth_noise(key, noise_clock(seconds, settings.slow_rate));
        const AnimationKey fast_key{key.low ^ 0x39ac17U, key.high};
        const auto fast =
            smooth_noise(fast_key, noise_clock(seconds, settings.fast_rate));
        const auto light = 1 +
            settings.strength * factor *
                (settings.slow_weight * slow +
                 (1 - settings.slow_weight) * fast);
        const AnimationKey colour_key{key.low ^ 0x47a672b1U, key.high};
        const auto chroma = settings.colour_variation * factor *
            smooth_noise(colour_key, noise_clock(seconds, settings.fast_rate));
        return {light * (1 + chroma), light * (1 - 0.12 * chroma),
                light * (1 - chroma)};
    }

    void AnimationClock::advance(double wall_delta, double speed) noexcept
    {
        if (picture_mode_ || !nonnegative_finite(wall_delta) ||
            !nonnegative_finite(speed))
            return;
        const auto updated = seconds_ + wall_delta * speed;
        if (std::isfinite(updated))
            seconds_ = updated;
    }

    void AnimationClock::set_picture_mode(bool enabled) noexcept
    {
        picture_mode_ = enabled;
    }

    double AnimationClock::seconds() const noexcept
    {
        return seconds_;
    }
} // namespace msc::core::appearance
