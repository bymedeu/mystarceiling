#pragma once

#include <expected>
#include "mystarceiling/core/maths/angle.hh"
namespace mystarceiling::core::maths
{
    enum class DeclinationError
    {
        NonFinite,
        OutOfRange
    };
    class Declination
    {
    public:
        [[nodiscard]] static std::expected<Declination, DeclinationError>
        try_from(const Angle& angle);

        [[nodiscard]] Angle get_angle() const noexcept;

    private:
        Declination(const Angle& angle);

        Angle angle_;
    };
} // namespace mystarceiling::core::maths
