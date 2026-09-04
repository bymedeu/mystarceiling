#pragma once

namespace msc::core::time
{

    /// A Julian date stored as two independently supplied parts.
    ///
    /// Splitting the value lets callers preserve precision by keeping a large
    /// reference epoch separate from a smaller offset.  No particular split
    /// or part ordering is required.
    class JulianDate
    {
    public:
        constexpr JulianDate(double part1, double part2) noexcept;

        [[nodiscard]] constexpr double part1() const noexcept;
        [[nodiscard]] constexpr double part2() const noexcept;

    private:
        double part1_;
        double part2_;
    };

    constexpr double JulianDate::part1() const noexcept
    {
        return part1_;
    }

    constexpr double JulianDate::part2() const noexcept
    {
        return part2_;
    }

    constexpr JulianDate::JulianDate(double part1, double part2) noexcept :
        part1_{part1}, part2_{part2}
    {}

} // namespace msc::core::time
