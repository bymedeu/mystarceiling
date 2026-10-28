#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <expected>
#include <mystarceiling/core/appearance/scintillation.hh>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <mystarceiling/core/maths/vector3.hh>
#include <span>
#include <string>

namespace msc::render
{
    struct TrialStar
    {
        core::maths::Vector3 enu;
        double magnitude{};
        core::astronomy::LinearRgb8 colour;
        core::appearance::AnimationKey animation_key;
    };

    struct TrialSettings
    {
        core::appearance::TwinkleSettings twinkle;
        double brightness{14};
        double limiting_magnitude{6};
        double core_width{0.4};
        double halo_width{1};
        double halo_fraction{0.03};
        double ray_fraction{0.04};
        double ray_length{1.5};
        double ray_width{0.20};
        double support_sigma{4};
        int ray_count{4};
        int width{1280};
        int height{720};
        bool picture_mode{};
    };

    [[nodiscard]] inline bool
    valid_trial_settings(const TrialSettings& value) noexcept
    {
        const std::array ranges{
            std::array{value.brightness, 0.01, 1000.0},
            std::array{value.limiting_magnitude, -30.0, 30.0},
            std::array{value.core_width, 0.3, 5.0},
            std::array{value.halo_width, 0.5, 32.0},
            std::array{value.halo_fraction, 0.0, 1.0},
            std::array{value.ray_fraction, 0.0, 1.0},
            std::array{value.ray_length, 0.5, 32.0},
            std::array{value.ray_width, 0.1, 3.0},
            std::array{value.support_sigma, 2.0, 6.0},
            std::array{double(value.ray_count), 3.0, 24.0},
            std::array{double(value.width), 64.0, 4096.0},
            std::array{double(value.height), 64.0, 2160.0}};
        return core::appearance::valid_twinkle_settings(value.twinkle) &&
            value.halo_fraction + value.ray_fraction <= 1.0 &&
            std::ranges::all_of(ranges,
                                [](const auto& range)
                                {
                                    return std::isfinite(range[0]) &&
                                        range[0] >= range[1] &&
                                        range[0] <= range[2];
                                });
    }
    [[nodiscard]] std::expected<void, std::string>
        show_appearance_trial(std::span<const TrialStar>, TrialSettings);
} // namespace msc::render
