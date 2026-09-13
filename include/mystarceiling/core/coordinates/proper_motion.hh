#pragma once

#include <cmath>
#include <expected>

namespace msc::core::coordinates
{
    enum class ProperMotionError
    {
        NonFinite
    };

    class ProperMotion
    {
    public:
        [[nodiscard]] static constexpr std::expected<ProperMotion,
                                                     ProperMotionError>
        try_from_milliarcseconds_per_year(double value) noexcept
        {
            if (!std::isfinite(value))
                return std::unexpected{ProperMotionError::NonFinite};
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

    /// Proper-motion components in milliarcseconds per Julian year.
    ///
    /// `right_ascension_cos_declination` is d(RA)/dt multiplied by
    /// cos(declination); it is not the raw right-ascension coordinate rate.
    struct ProperMotionComponents
    {
        ProperMotion right_ascension_cos_declination;
        ProperMotion declination;
    };
} // namespace msc::core::coordinates
