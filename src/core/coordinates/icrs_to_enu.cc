#include <cmath>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/maths/vector3.hh>

namespace msc::core::coordinates
{
    using maths::Angle;
    using maths::Vector3;

    Vector3 icrs_to_enu(IcrsDirection star_direction,
                        ObserverLocation observer_location,
                        Angle greenwich_sidereal_angle)
    {
        const auto alpha =
            star_direction.get_right_ascension().get_angle().to_radian();
        const auto delta =
            star_direction.get_declination().get_angle().to_radian();
        const auto latitude =
            observer_location.get_latitude().get_angle().to_radian();
        const auto longitude =
            observer_location.get_longitude().get_angle().to_radian();

        // Local hour angle
        const auto H = greenwich_sidereal_angle.to_radian() + longitude - alpha;

        const double east = -std::cos(delta) * std::sin(H);
        const double north = std::sin(delta) * std::cos(latitude) -
            std::cos(delta) * std::cos(H) * std::sin(latitude);
        const double up = std::sin(delta) * std::sin(latitude) +
            std::cos(delta) * std::cos(H) * std::cos(latitude);

        return Vector3(east, north, up);
    }

} // namespace msc::core::coordinates
