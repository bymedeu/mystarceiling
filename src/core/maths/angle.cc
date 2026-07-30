#include <mystarceiling/core/maths/angle.hh>
#include <numbers>

namespace mystarceiling::core::maths
{

    Angle::Angle(double rad) : rad_{rad}
    {
    }

    Angle Angle::from_radian(double radian)
    {
        return Angle(radian);
    }
    Angle Angle::from_degrees(double degrees)
    {
        double radian = degrees / 180 * std::numbers::pi;
        return Angle(radian);
    }
    Angle Angle::from_hours(double hours)
    {
        double radian = hours / 12 * std::numbers::pi;
        return Angle(radian);
    }

    double Angle::to_radian() const
    {
        return rad_;
    }
    double Angle::to_degrees() const
    {
        return rad_ * 180 / std::numbers::pi;
    }
    double Angle::to_hours() const
    {
        return rad_ * 12 / std::numbers::pi;
    }


} // namespace mystarceiling::core::maths
