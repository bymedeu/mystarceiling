#pragma once

#include <cmath>
#include <expected>
#include <mystarceiling/core/maths/angle.hh>
#include <numbers>

namespace msc::core::coordinates
{
    enum class DeclinationError
    {
        NonFinite,
        OutOfRange
    };

    class Declination
    {
    public:
        [[nodiscard]] constexpr static std::expected<Declination,
                                                     DeclinationError>
        try_from(const maths::Angle& angle) noexcept;

        [[nodiscard]] constexpr maths::Angle get_angle() const noexcept;

    private:
        explicit constexpr Declination(const maths::Angle& angle) noexcept;

        maths::Angle angle_;
    };

    constexpr std::expected<Declination, DeclinationError>
    Declination::try_from(const maths::Angle& angle) noexcept
    {
        const auto rad = angle.to_radian();
        constexpr double halfpi = std::numbers::pi / 2;
        if (!std::isfinite(rad))
            return std::unexpected{DeclinationError::NonFinite};

        if (rad < -halfpi || rad > halfpi)
            return std::unexpected{DeclinationError::OutOfRange};

        return Declination{angle};
    }

    constexpr maths::Angle Declination::get_angle() const noexcept
    {
        return angle_;
    }

    constexpr Declination::Declination(const maths::Angle& angle) noexcept :
        angle_{angle}
    {}
} // namespace msc::core::coordinates
