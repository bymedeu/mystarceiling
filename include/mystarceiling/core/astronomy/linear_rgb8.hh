#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <expected>

namespace msc::core::astronomy
{
    enum class ColourError
    {
        InvalidChannel
    };

    // Linear sRGB channel proportions, not magnitude or display-encoded sRGB.
    struct LinearRgb8
    {
        std::uint8_t red{};
        std::uint8_t green{};
        std::uint8_t blue{};

        [[nodiscard]] static std::expected<LinearRgb8, ColourError>
        try_from_linear(const std::array<double, 3>& channels) noexcept
        {
            if (std::ranges::any_of(channels,
                                    [](double value)
                                    {
                                        return !std::isfinite(value) ||
                                            value < 0 || value > 1;
                                    }))
                return std::unexpected{ColourError::InvalidChannel};
            return LinearRgb8{
                static_cast<std::uint8_t>(std::lround(255 * channels[0])),
                static_cast<std::uint8_t>(std::lround(255 * channels[1])),
                static_cast<std::uint8_t>(std::lround(255 * channels[2]))};
        }

        constexpr bool operator==(const LinearRgb8&) const noexcept = default;
    };

    static_assert(sizeof(LinearRgb8) == 3);
} // namespace msc::core::astronomy
