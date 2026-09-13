#pragma once

#include <cstdint>
#include <mystarceiling/catalogue/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/time/reference_epoch.hh>

namespace msc::catalogue
{
    using core::coordinates::IcrsDirection;
    using core::coordinates::ProperMotionComponents;
    using core::time::ReferenceEpoch;

    class Star
    {
    public:
        constexpr Star(std::uint64_t source_id,
                       ReferenceEpoch reference_epoch,
                       IcrsDirection direction,
                       ProperMotionComponents proper_motion, Parallax parallax,
                       ApparentMagnitude magnitude, ColourIndex colour) noexcept
            :
            source_id_{source_id}, reference_epoch_{reference_epoch},
            direction_{direction},
            proper_motion_{proper_motion}, parallax_{parallax},
            magnitude_{magnitude}, colour_{colour}
        {}

        [[nodiscard]] constexpr std::uint64_t source_id() const noexcept
        {
            return source_id_;
        }
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
        [[nodiscard]] constexpr ColourIndex colour() const noexcept
        {
            return colour_;
        }

    private:
        std::uint64_t source_id_;
        ReferenceEpoch reference_epoch_;
        IcrsDirection direction_;
        ProperMotionComponents proper_motion_;
        Parallax parallax_;
        ApparentMagnitude magnitude_;
        ColourIndex colour_;
    };
} // namespace msc::catalogue
