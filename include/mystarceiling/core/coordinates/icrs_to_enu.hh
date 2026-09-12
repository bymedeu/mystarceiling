#pragma once

#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/maths/vector3.hh>
namespace msc::core::coordinates
{

    [[nodiscard]] maths::Vector3
    icrs_to_enu(IcrsDirection star_direction,
                ObserverLocation observer_location,
                maths::Angle greenwich_sidereal_angle);

}
