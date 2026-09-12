#pragma once

#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/vector3.hh>

namespace msc::core::coordinates
{
    class IcrsDirection
    {
    public:
        [[nodiscard]] constexpr IcrsDirection(RightAscension ra,
                                              Declination dec) noexcept;

        [[nodiscard]] constexpr Declination get_declination() const noexcept;
        [[nodiscard]] constexpr RightAscension
        get_right_ascension() const noexcept;
        [[nodiscard]] maths::Vector3 to_unit_vector() const;

    private:
        RightAscension ra_;
        Declination dec_;
    };

    constexpr IcrsDirection::IcrsDirection(RightAscension right_ascension,
                                           Declination declination) noexcept :
        ra_{right_ascension}, dec_{declination}
    {}

    constexpr RightAscension IcrsDirection::get_right_ascension() const noexcept
    {
        return ra_;
    }

    constexpr Declination IcrsDirection::get_declination() const noexcept
    {
        return dec_;
    }
} // namespace msc::core::coordinates
