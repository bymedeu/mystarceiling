#include <mystarceiling/core/coordinates/propagate_proper_motion.hh>

#include <algorithm>
#include <cmath>
#include <expected>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <numbers>

namespace msc::core::coordinates
{
    using maths::Angle;

    std::expected<IcrsDirection, ProperMotionPropagationError>
    propagate_proper_motion(IcrsDirection direction,
                            ProperMotionComponents motion,
                            time::ReferenceEpoch reference_epoch,
                            time::ReferenceEpoch target_epoch) noexcept
    {
        const double elapsed_years =
            target_epoch.julian_year() - reference_epoch.julian_year();
        const double right_ascension_motion =
            motion.right_ascension_cos_declination.milliarcseconds_per_year();
        const double declination_motion =
            motion.declination.milliarcseconds_per_year();

        if (!std::isfinite(elapsed_years))
            return std::unexpected{
                ProperMotionPropagationError::NonFiniteEpochDifference};
        if (elapsed_years == 0.0 ||
            (right_ascension_motion == 0.0 && declination_motion == 0.0))
            return direction;

        constexpr double milliarcseconds_to_radians =
            std::numbers::pi / (180.0 * 3'600'000.0);
        const double east =
            right_ascension_motion * milliarcseconds_to_radians * elapsed_years;
        const double north =
            declination_motion * milliarcseconds_to_radians * elapsed_years;
        if (!std::isfinite(east) || !std::isfinite(north))
            return std::unexpected{
                ProperMotionPropagationError::NonFiniteDisplacement};

        const double right_ascension =
            direction.get_right_ascension().get_angle().to_radian();
        const double declination =
            direction.get_declination().get_angle().to_radian();
        const double sin_right_ascension = std::sin(right_ascension);
        const double cos_right_ascension = std::cos(right_ascension);
        const double sin_declination = std::sin(declination);
        // cos(pi/2) is not exactly zero in floating point.
        const double cos_declination =
            std::abs(declination) == std::numbers::pi / 2
            ? 0.0
            : std::cos(declination);
        double x = cos_declination * cos_right_ascension -
            east * sin_right_ascension -
            north * sin_declination * cos_right_ascension;
        double y = cos_declination * sin_right_ascension +
            east * cos_right_ascension -
            north * sin_declination * sin_right_ascension;
        double z = sin_declination + north * cos_declination;
        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z))
            return std::unexpected{
                ProperMotionPropagationError::NonFiniteDisplacement};

        const double scale = std::max({std::abs(x), std::abs(y), std::abs(z)});
        if (scale == 0.0)
            return std::unexpected{
                ProperMotionPropagationError::InvalidDirection};
        x /= scale;
        y /= scale;
        z /= scale;
        const double length = std::hypot(x, y, z);
        if (!std::isfinite(length) || length == 0.0)
            return std::unexpected{
                ProperMotionPropagationError::InvalidDirection};
        x /= length;
        y /= length;
        z /= length;
        const double xy = std::hypot(x, y);
        double alpha = xy == 0.0 ? 0.0 : std::atan2(y, x);
        constexpr double turn = 2 * std::numbers::pi;
        if (alpha < 0.0)
            alpha += turn;
        // Adding a tiny negative angle to one turn can round to the endpoint.
        if (!(alpha < turn))
            alpha = 0.0;
        const auto result_ra =
            RightAscension::try_from(Angle::from_radian(alpha));
        const auto result_dec =
            Declination::try_from(Angle::from_radian(std::atan2(z, xy)));
        if (!result_ra || !result_dec)
            return std::unexpected{
                ProperMotionPropagationError::InvalidDirection};
        return IcrsDirection{*result_ra, *result_dec};
    }
} // namespace msc::core::coordinates
