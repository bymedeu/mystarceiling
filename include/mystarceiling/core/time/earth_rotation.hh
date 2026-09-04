#pragma once

#include "mystarceiling/core/maths/angle.hh"
#include "mystarceiling/core/time/julian_date.hh"
namespace msc::core::time
{

    [[nodiscard]] maths::Angle earth_rotation_angle(JulianDate ut1) noexcept;

}
