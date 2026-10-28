#include <mystarceiling/core/appearance/scintillation.hh>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <numbers>
#include <numeric>

namespace msc::core::appearance
{
    namespace
    {
        constexpr double tau = 2 * std::numbers::pi;
        using Spectrum =
            std::array<std::array<double, 2>, scintillation_band_count>;

        const Spectrum& spectrum() noexcept
        {
            static const auto value = []
            {
                Spectrum result{};
                double variance{};
                const auto ratio = std::pow(256.0 / 0.5, 1.0 / result.size());
                double lower = 0.5;
                for (auto& band : result)
                {
                    const auto upper = lower * ratio;
                    const auto frequency = std::sqrt(lower * upper);
                    // Broad small-pupil surrogate; the 45 Hz knee is a trial
                    // choice, not a measured wind/turbulence profile.
                    const auto power =
                        1 / (1 + std::pow(frequency / 45, 8.0 / 3));
                    band = {frequency, std::sqrt(2 * power * (upper - lower))};
                    variance += band[1] * band[1] / 2;
                    lower = upper;
                }
                for (auto& band : result)
                    band[1] /= std::sqrt(variance);
                return result;
            }();
            return value;
        }

        double peak_bound() noexcept
        {
            return std::accumulate(spectrum().begin(), spectrum().end(), 0.0,
                                   [](double sum, const auto& band)
                                   { return sum + band[1]; });
        }

        double box_average(double frequency, double exposure) noexcept
        {
            const auto x = std::numbers::pi * frequency * exposure;
            if (!std::isfinite(x))
                return 0;
            if (std::abs(x) < 1e-4)
                return 1 - x * x / 6;
            return std::sin(x) / x;
        }

        bool nonnegative_finite(double value) noexcept
        {
            return std::isfinite(value) && value >= 0;
        }

        std::uint32_t mixed(std::uint32_t value) noexcept
        {
            value = (value ^ (value >> 16U)) * 0x21f0aaadU;
            value = (value ^ (value >> 15U)) * 0x735a2d97U;
            return value ^ (value >> 15U);
        }
    } // namespace

    bool valid_twinkle_settings(const TwinkleSettings& settings) noexcept
    {
        return nonnegative_finite(settings.strength) &&
            settings.strength * peak_bound() < 0.99 &&
            nonnegative_finite(settings.zenith_fraction) &&
            settings.zenith_fraction <= 1 &&
            nonnegative_finite(settings.colour_delay) &&
            settings.colour_delay <= 1;
    }

    std::uint32_t hash_noise(AnimationKey key, std::uint32_t counter) noexcept
    {
        return mixed(key.low ^ mixed(key.high) ^ (counter * 0x9e3779b9U));
    }

    ScintillationFrame scintillation_frame(double seconds,
                                           double exposure) noexcept
    {
        ScintillationFrame result{};
        const auto duration = std::min(seconds, exposure);
        const auto midpoint = seconds - duration / 2;
        for (std::size_t index = 0; index < result.size(); ++index)
        {
            const auto& band = spectrum()[index];
            const auto phase = std::fmod(midpoint, 1 / band[0]) * band[0];
            result[index] = {
                static_cast<float>(phase),
                static_cast<float>(band[1] * box_average(band[0], duration)),
                static_cast<float>(band[0])};
        }
        return result;
    }

    std::array<double, 3>
    sample_twinkle(AnimationKey key, const ScintillationFrame& frame,
                   double sine_elevation,
                   const TwinkleSettings& settings) noexcept
    {
        const auto horizon = 1 - std::clamp(sine_elevation, 0.0, 1.0);
        const auto factor = settings.zenith_fraction +
            (1 - settings.zenith_fraction) * horizon * horizon;
        std::array<double, 3> result{1, 1, 1};
        for (std::size_t index = 0; index < frame.size(); ++index)
        {
            const auto& band = frame[index];
            const auto offset =
                static_cast<double>(
                    hash_noise(key, static_cast<std::uint32_t>(index) + 719U) >>
                    8U) /
                16777216.0;
            const auto phase = tau * (band[0] + offset);
            const auto delay = tau * band[2] * settings.colour_delay * horizon;
            for (std::size_t channel = 0; channel < result.size(); ++channel)
                result[channel] += settings.strength * factor * band[1] *
                    std::cos(phase +
                             (static_cast<double>(channel) - 1) * delay);
        }
        return result;
    }

    void AnimationClock::advance(double wall_delta) noexcept
    {
        if (picture_mode_ || !std::isfinite(wall_delta) || wall_delta <= 0)
            return;
        const auto updated = seconds_ + wall_delta;
        if (std::isfinite(updated))
        {
            seconds_ = updated;
            exposure_ = wall_delta;
        }
    }

    void AnimationClock::set_picture_mode(bool enabled) noexcept
    {
        picture_mode_ = enabled;
    }

    double AnimationClock::seconds() const noexcept
    {
        return seconds_;
    }

    double AnimationClock::exposure() const noexcept
    {
        return exposure_;
    }
} // namespace msc::core::appearance
