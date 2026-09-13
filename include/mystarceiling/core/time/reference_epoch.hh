#pragma once

#include <cmath>
#include <expected>

namespace msc::core::time
{
    enum class ReferenceEpochError
    {
        NonFinite
    };

    // Finite Julian epoch: J2000.0 plus years of exactly 365.25 days.
    // The time scale is supplied by the caller, not encoded in this value.
    class ReferenceEpoch
    {
    public:
        [[nodiscard]] static constexpr std::expected<ReferenceEpoch,
                                                     ReferenceEpochError>
        try_from_julian_year(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{ReferenceEpochError::NonFinite};
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
} // namespace msc::core::time
