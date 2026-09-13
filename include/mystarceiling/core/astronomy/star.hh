#pragma once

#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <optional>

namespace msc::core::astronomy
{
    using core::coordinates::IcrsDirection;
    using core::coordinates::ProperMotionComponents;
    using core::time::ReferenceEpoch;

    // Source-independent validated stellar state; photometry is G-like and
    // BP-RP.
    class Star
    {
    public:
        constexpr Star(ReferenceEpoch reference_epoch, IcrsDirection direction,
                       ProperMotionComponents proper_motion, Parallax parallax,
                       ApparentMagnitude magnitude,
                       std::optional<ColourIndex> colour) noexcept :
            reference_epoch_{reference_epoch}, direction_{direction},
            proper_motion_{proper_motion}, parallax_{parallax},
            magnitude_{magnitude}, colour_{colour}
        {}

        [[nodiscard]] constexpr ReferenceEpoch reference_epoch() const noexcept
        {
            return reference_epoch_;
        }
        [[nodiscard]] constexpr IcrsDirection direction() const noexcept
        {
            return direction_;
        }
        [[nodiscard]] constexpr ProperMotionComponents
        proper_motion() const noexcept
        {
            return proper_motion_;
        }
        [[nodiscard]] constexpr Parallax parallax() const noexcept
        {
            return parallax_;
        }
        [[nodiscard]] constexpr ApparentMagnitude magnitude() const noexcept
        {
            return magnitude_;
        }
        // Gaia BP-RP only; unavailable for the Hipparcos supplement.
        [[nodiscard]] constexpr std::optional<ColourIndex>
        colour() const noexcept
        {
            return colour_;
        }

    private:
        ReferenceEpoch reference_epoch_;
        IcrsDirection direction_;
        ProperMotionComponents proper_motion_;
        Parallax parallax_;
        ApparentMagnitude magnitude_;
        std::optional<ColourIndex> colour_;
    };
} // namespace msc::core::astronomy
