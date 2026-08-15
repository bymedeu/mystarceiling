#pragma once

#include <array>
namespace mystarceiling::core::maths
{
    class Vector3
    {
    public:
        [[nodiscard]] constexpr Vector3(double x, double y, double z) noexcept;
        [[nodiscard]] constexpr double x() const noexcept;
        [[nodiscard]] constexpr double y() const noexcept;
        [[nodiscard]] constexpr double z() const noexcept;

    private:
        std::array<double, 3> coords_;
    };

    [[nodiscard]] constexpr double Vector3::x() const noexcept
    {
        return coords_[0];
    }
    [[nodiscard]] constexpr double Vector3::y() const noexcept
    {
        return coords_[1];
    }
    [[nodiscard]] constexpr double Vector3::z() const noexcept
    {
        return coords_[2];
    }

    [[nodiscard]] constexpr Vector3::Vector3(double x, double y,
                                             double z) noexcept :
        coords_{{x, y, z}}
    {}

} // namespace mystarceiling::core::maths
