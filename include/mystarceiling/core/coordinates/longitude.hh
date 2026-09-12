#pragma once

#include <cmath>
#include <expected>
#include <numbers>
#include <mystarceiling/core/maths/angle.hh>

namespace msc::core::coordinates
{
    enum class LongitudeError
    {
        NonFinite,
        OutOfRange
    };

    class Longitude
    {
    public:
        [[nodiscard]] constexpr static std::expected<Longitude, LongitudeError>
        try_from(const maths::Angle& angle) noexcept;

        [[nodiscard]] constexpr maths::Angle get_angle() const noexcept;

    private:
        constexpr Longitude(maths::Angle angle) noexcept;

        maths::Angle angle_;
    };

    constexpr std::expected<Longitude, LongitudeError>
    Longitude::try_from(const maths::Angle& angle) noexcept
    {
        const auto rad = angle.to_radian();
        constexpr double pi = std::numbers::pi;
        if (!std::isfinite(rad))
            return std::unexpected{LongitudeError::NonFinite};

        if (rad < -pi || rad >= pi)
            return std::unexpected{LongitudeError::OutOfRange};

        return Longitude{angle};
    }

    constexpr maths::Angle Longitude::get_angle() const noexcept
    {
        return angle_;
    }

    constexpr Longitude::Longitude(maths::Angle angle) noexcept : angle_{angle}
    {}
} // namespace msc::core::coordinates
