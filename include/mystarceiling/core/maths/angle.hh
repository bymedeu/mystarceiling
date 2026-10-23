#pragma once

#include <numbers>
namespace msc::core::maths
{
    class Angle
    {
    public:
        constexpr static Angle from_radian(double radian) noexcept;
        constexpr static Angle from_degrees(double degrees) noexcept;
        constexpr static Angle from_hours(double hours) noexcept;

        [[nodiscard]] constexpr double to_radian() const noexcept;
        [[nodiscard]] constexpr double to_degrees() const noexcept;
        [[nodiscard]] constexpr double to_hours() const noexcept;


    private:
        explicit constexpr Angle(double rad) noexcept;
        double rad_;
    };

    constexpr Angle::Angle(double rad) noexcept : rad_{rad}
    {}

    constexpr Angle Angle::from_radian(double radian) noexcept
    {
        return Angle(radian);
    }
    constexpr Angle Angle::from_degrees(double degrees) noexcept
    {
        double radian = degrees / 180 * std::numbers::pi;
        return Angle(radian);
    }
    constexpr Angle Angle::from_hours(double hours) noexcept
    {
        double radian = hours / 12 * std::numbers::pi;
        return Angle(radian);
    }

    constexpr double Angle::to_radian() const noexcept
    {
        return rad_;
    }
    constexpr double Angle::to_degrees() const noexcept
    {
        return rad_ * 180 / std::numbers::pi;
    }
    constexpr double Angle::to_hours() const noexcept
    {
        return rad_ * 12 / std::numbers::pi;
    }
} // namespace msc::core::maths
