#include <cmath>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include "mystarceiling/core/maths/vector3.hh"

namespace mystarceiling::core::coordinates
{
    maths::Vector3 IcrsDirection::to_unit_vector() const
    {
        const double alpha = ra_.get_angle().to_radian();
        const double delta = dec_.get_angle().to_radian();
        const double cos_declination = std::cos(delta);

        const double x = cos_declination * std::cos(alpha);
        const double y = cos_declination * std::sin(alpha);
        const double z = std::sin(delta);

        return maths::Vector3(x, y, z);
    }
} // namespace mystarceiling::core::coordinates
