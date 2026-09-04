#pragma once

#include <cmath>
#include <expected>
#include <numbers>
#include "mystarceiling/core/maths/angle.hh"

namespace msc::core::coordinates
{
    enum class LatitudeError
    {
        NonFinite,
        OutOfRange
    };

    class Latitude
    {
    public:
        [[nodiscard]] constexpr static std::expected<Latitude, LatitudeError>
        try_from(const maths::Angle& angle) noexcept;

        [[nodiscard]] constexpr maths::Angle get_angle() const noexcept;

    private:
        constexpr Latitude(maths::Angle angle) noexcept;

        maths::Angle angle_;
    };

    constexpr std::expected<Latitude, LatitudeError>
    Latitude::try_from(const maths::Angle& angle) noexcept
    {
        const auto rad = angle.to_radian();
        constexpr double halfpi = std::numbers::pi / 2;
        if (!std::isfinite(rad))
            return std::unexpected{LatitudeError::NonFinite};

        if (rad < -halfpi || rad > halfpi)
            return std::unexpected{LatitudeError::OutOfRange};

        return Latitude{angle};
    }

    constexpr maths::Angle Latitude::get_angle() const noexcept
    {
        return angle_;
    }

    constexpr Latitude::Latitude(maths::Angle angle) noexcept : angle_{angle}
    {}
} // namespace msc::core::coordinates
