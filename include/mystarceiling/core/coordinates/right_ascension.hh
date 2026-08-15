/**
 * @file right_ascension.hh
 * How right_ascension data is being encoded
 */

#pragma once

#include <cmath>
#include <expected>
#include <numbers>
#include "mystarceiling/core/maths/angle.hh"

namespace mystarceiling::core::coordinates
{
    enum class RightAscensionError
    {
        NonFinite,
        OutOfRange
    };

    class RightAscension
    {
    public:
        [[nodiscard]] constexpr static std::expected<RightAscension,
                                                     RightAscensionError>
        try_from(const maths::Angle& angle) noexcept;

        [[nodiscard]] constexpr maths::Angle get_angle() const noexcept;

    private:
        constexpr RightAscension(const maths::Angle& angle) noexcept;

        maths::Angle angle_;
    };

    constexpr std::expected<RightAscension, RightAscensionError>
    RightAscension::try_from(const maths::Angle& angle) noexcept
    {
        const auto rad = angle.to_radian();
        if (!std::isfinite(rad))
            return std::unexpected{RightAscensionError::NonFinite};

        if (rad < 0 || rad >= std::numbers::pi * 2)
            return std::unexpected{RightAscensionError::OutOfRange};

        return RightAscension{angle};
    }

    constexpr maths::Angle RightAscension::get_angle() const noexcept
    {
        return angle_;
    }

    constexpr RightAscension::RightAscension(
        const maths::Angle& angle) noexcept : angle_{angle}
    {}
} // namespace mystarceiling::core::coordinates
