#pragma once

#include <cmath>
#include <expected>

namespace msc::catalogue
{
    enum class MeasurementError
    {
        NonFinite
    };

    class ReferenceEpoch
    {
    public:
        [[nodiscard]] static constexpr std::expected<ReferenceEpoch,
                                                     MeasurementError>
        try_from_julian_year(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{MeasurementError::NonFinite};
            return ReferenceEpoch{value};
        }

        [[nodiscard]] constexpr double julian_year() const noexcept
        {
            return value_;
        }

    private:
        explicit constexpr ReferenceEpoch(double value) noexcept : value_{value}
        {}
        double value_;
    };

    class ProperMotion
    {
    public:
        [[nodiscard]] static constexpr std::expected<ProperMotion,
                                                     MeasurementError>
        try_from_milliarcseconds_per_year(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{MeasurementError::NonFinite};
            return ProperMotion{value};
        }

        [[nodiscard]] constexpr double milliarcseconds_per_year() const noexcept
        {
            return value_;
        }

    private:
        explicit constexpr ProperMotion(double value) noexcept : value_{value}
        {}
        double value_;
    };

    class Parallax
    {
    public:
        [[nodiscard]] static constexpr std::expected<Parallax, MeasurementError>
        try_from_milliarcseconds(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{MeasurementError::NonFinite};
            return Parallax{value};
        }

        [[nodiscard]] constexpr double milliarcseconds() const noexcept
        {
            return value_;
        }

    private:
        explicit constexpr Parallax(double value) noexcept : value_{value}
        {}
        double value_;
    };

    class ApparentMagnitude
    {
    public:
        [[nodiscard]] static constexpr std::expected<ApparentMagnitude,
                                                     MeasurementError>
        try_from_gaia_g(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{MeasurementError::NonFinite};
            return ApparentMagnitude{value};
        }

        [[nodiscard]] constexpr double gaia_g() const noexcept
        {
            return value_;
        }

    private:
        explicit constexpr ApparentMagnitude(double value) noexcept :
            value_{value}
        {}
        double value_;
    };

    class ColourIndex
    {
    public:
        [[nodiscard]] static constexpr std::expected<ColourIndex,
                                                     MeasurementError>
        try_from_gaia_bp_minus_rp(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{MeasurementError::NonFinite};
            return ColourIndex{value};
        }

        [[nodiscard]] constexpr double gaia_bp_minus_rp() const noexcept
        {
            return value_;
        }

    private:
        explicit constexpr ColourIndex(double value) noexcept : value_{value}
        {}
        double value_;
    };
} // namespace msc::catalogue
