#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace msc::core::appearance
{
    struct AnimationKey
    {
        std::uint32_t low{};
        std::uint32_t high{};
    };

    // Internal visual-trial coefficients, not calibrated observing conditions.
    struct TwinkleSettings
    {
        double strength{0.12}; // Unaveraged horizon RMS of each channel.
        double zenith_fraction{0.25};
        double colour_delay{
            0.0015}; // Adjacent-channel delay at horizon, seconds.
    };

    inline constexpr std::size_t scintillation_band_count = 24;
    // Per-band phase in cycles, exposure-averaged amplitude, frequency in Hz.
    // Compact float uniforms are shared by all sources; no per-star history.
    using ScintillationFrame =
        std::array<std::array<float, 3>, scintillation_band_count>;

    [[nodiscard]] bool valid_twinkle_settings(const TwinkleSettings&) noexcept;
    [[nodiscard]] std::uint32_t hash_noise(AnimationKey,
                                           std::uint32_t counter) noexcept;
    // Average the linear signal over [seconds-exposure, seconds], clipping the
    // interval to the animation origin. Zero exposure samples the instant.
    // Requires finite nonnegative time/exposure. Phases are reduced on the CPU.
    [[nodiscard]] ScintillationFrame
    scintillation_frame(double seconds, double exposure) noexcept;
    // Pure CPU reference for the shader. Requires valid settings, a frame made
    // above, and a finite elevation sine. Returns positive, mean-one gains.
    [[nodiscard]] std::array<double, 3>
    sample_twinkle(AnimationKey, const ScintillationFrame&,
                   double sine_elevation, const TwinkleSettings& = {}) noexcept;

    class AnimationClock
    {
    public:
        // Picture mode freezes both time and the last exposure interval.
        // Invalid, negative, zero or overflowing updates preserve the state.
        void advance(double wall_delta) noexcept;
        void set_picture_mode(bool enabled) noexcept;
        [[nodiscard]] double seconds() const noexcept;
        [[nodiscard]] double exposure() const noexcept;

    private:
        double seconds_{};
        double exposure_{1.0 / 60};
        bool picture_mode_{};
    };
} // namespace msc::core::appearance
