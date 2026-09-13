#pragma once

#include <cmath>
#include <expected>

namespace msc::core::astronomy
{
    enum class MeasurementError
    {
        NonFinite
    };

    // Finite measured parallax in mas; zero and negative values are valid.
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

    // G-like apparent magnitude: Gaia G or a source-prepared estimate of G.
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

    // Gaia BP-RP colour index, not display RGB or Johnson B-V.
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
} // namespace msc::core::astronomy
