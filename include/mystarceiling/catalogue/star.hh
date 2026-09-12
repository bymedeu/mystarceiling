#pragma once

#include <cstdint>
#include <mystarceiling/catalogue/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>

namespace msc::catalogue
{
    class Star
    {
    public:
        constexpr Star(std::uint64_t source_id, ReferenceEpoch reference_epoch,
                       core::coordinates::IcrsDirection direction,
                       ProperMotion right_ascension_motion,
                       ProperMotion declination_motion, Parallax parallax,
                       ApparentMagnitude magnitude, ColourIndex colour) noexcept
            :
            source_id_{source_id}, reference_epoch_{reference_epoch},
            direction_{direction},
            right_ascension_motion_{right_ascension_motion},
            declination_motion_{declination_motion}, parallax_{parallax},
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
        [[nodiscard]] constexpr core::coordinates::IcrsDirection
        direction() const noexcept
        {
            return direction_;
        }
        [[nodiscard]] constexpr ProperMotion
        right_ascension_motion() const noexcept
        {
            return right_ascension_motion_;
        }
        [[nodiscard]] constexpr ProperMotion declination_motion() const noexcept
        {
            return declination_motion_;
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
        core::coordinates::IcrsDirection direction_;
        ProperMotion right_ascension_motion_;
        ProperMotion declination_motion_;
        Parallax parallax_;
        ApparentMagnitude magnitude_;
        ColourIndex colour_;
    };
} // namespace msc::catalogue
