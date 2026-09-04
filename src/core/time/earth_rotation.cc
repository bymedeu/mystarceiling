#include <algorithm>
#include <cmath>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>
#include <numbers>

namespace msc::core::time
{

    maths::Angle earth_rotation_angle(JulianDate ut1) noexcept
    {
        const double small = std::min(ut1.part1(), ut1.part2());
        const double large = std::max(ut1.part1(), ut1.part2());
        const double days_since_j2000 = small + (large - 2451545.0);
        const double fract =
            std::fmod(ut1.part1(), 1) + std::fmod(ut1.part2(), 1);

        const double turns =
            0.7790572732640 + fract + 0.00273781191135448 * days_since_j2000;

        double radians =
            std::fmod(turns * 2 * std::numbers::pi, 2 * std::numbers::pi);
        if (radians < 0)
            radians += 2 * std::numbers::pi;

        return maths::Angle::from_radian(radians);
    }

} // namespace msc::core::time
