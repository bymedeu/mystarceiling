#pragma once

#include <array>
#include <cstdint>

namespace msc::core::appearance
{
    struct AnimationKey
    {
        std::uint32_t low{};
        std::uint32_t high{};
    };

    struct TwinkleSettings
    {
        double strength{0.65};
        double colour_variation{0.4};
        double zenith_fraction{0.1};
        double slow_rate{2};
        double fast_rate{9};
        double slow_weight{0.7};
    };

    struct NoiseClock
    {
        std::uint32_t interval{};
        float fraction{};
    };

    [[nodiscard]] bool valid_twinkle_settings(const TwinkleSettings&) noexcept;
    // Nonnegative finite seconds and rate are required. The interval wraps
    // modulo 2^32; fractional time is retained rather than restarting a period.
    [[nodiscard]] NoiseClock noise_clock(double seconds, double rate) noexcept;
    [[nodiscard]] std::uint32_t hash_noise(AnimationKey,
                                           std::uint32_t counter) noexcept;
    // A deterministic, smooth value in [-1, 1], with a per-key phase offset.
    [[nodiscard]] double smooth_noise(AnimationKey, NoiseClock) noexcept;
    // Visual trial multipliers, not a model of atmospheric transmission.
    // Requires valid settings and finite nonnegative seconds/elevation sine.
    [[nodiscard]] std::array<double, 3>
    sample_twinkle(AnimationKey, double seconds, double sine_elevation,
                   const TwinkleSettings&) noexcept;

    class AnimationClock
    {
    public:
        // Frozen clocks ignore elapsed wall time. Negative/nonfinite inputs
        // and updates that would overflow are ignored, preserving the state.
        void advance(double wall_delta, double speed) noexcept;
        void set_picture_mode(bool enabled) noexcept;
        [[nodiscard]] double seconds() const noexcept;

    private:
        double seconds_{};
        bool picture_mode_{};
    };
} // namespace msc::core::appearance
