#pragma once

namespace mystarceiling::core::maths
{
    class Angle
    {
    public:
        static Angle from_radian(double radian);
        static Angle from_degrees(double degrees);
        static Angle from_hours(double hours);

        [[nodiscard]] double to_radian() const;
        [[nodiscard]] double to_degrees() const;
        [[nodiscard]] double to_hours() const;


    private:
        Angle(double rad);
        double rad_;
    };

} // namespace mystarceiling::core::maths
