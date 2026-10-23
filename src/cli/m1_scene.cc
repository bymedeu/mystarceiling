#include "cli/m1_scene.hh"

#include <algorithm>
#include <cmath>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/coordinates/propagate_proper_motion.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/projection/zenith_perspective.hh>
#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <mystarceiling/render/opengl_renderer.hh>
#include <optional>
#include <span>

namespace msc::cli
{
    using catalogue::CatalogueSource;
    using catalogue::CatalogueStar;
    using core::coordinates::icrs_to_enu;
    using core::coordinates::Latitude;
    using core::coordinates::Longitude;
    using core::coordinates::ObserverLocation;
    using core::coordinates::propagate_proper_motion;
    using core::maths::Angle;
    using core::projection::project_zenith_perspective;
    using core::time::earth_rotation_angle;
    using core::time::JulianDate;
    using core::time::ReferenceEpoch;
    using render::ScreenPoint;

    float m1_display_intensity(double magnitude) noexcept
    {
        // Fixed G=-1.5 white reference; square-root flux compression, without
        // the old 0.15 floor that collapsed the fainter stars to identical
        // intensities.
        return static_cast<float>(
            std::clamp(std::pow(10.0, -0.2 * (magnitude + 1.5)), 0.0, 1.0));
    }

    namespace
    {
        std::optional<core::coordinates::IcrsDirection>
        scene_direction(const CatalogueStar& entry, ReferenceEpoch target,
                        M1Scene& scene)
        {
            const auto& star = entry.star();
            auto direction = star.direction();
            if (entry.source_catalogue() == CatalogueSource::Hipparcos2)
            {
                const auto propagated =
                    propagate_proper_motion(direction, star.proper_motion(),
                                            star.reference_epoch(), target);
                if (!propagated)
                {
                    ++scene.propagation_failures;
                    return std::nullopt;
                }
                direction = *propagated;
            }
            return direction;
        }
    } // namespace

    M1Scene make_m1_scene(std::span<const CatalogueStar> stars)
    {
        const ObserverLocation observer{
            *Latitude::try_from(Angle::from_degrees(43.6045)),
            *Longitude::try_from(Angle::from_degrees(1.4440))};
        // Fixed M1 approximation: UT1 = UTC, distinct from the J2016 motion
        // target.
        const auto rotation = earth_rotation_angle(JulianDate{2457388.5, 0.0});
        const auto target = *ReferenceEpoch::try_from_julian_year(2016.0);
        M1Scene scene;
        scene.points.reserve(stars.size());
        for (const auto& entry : stars)
        {
            const auto& star = entry.star();
            const auto direction = scene_direction(entry, target, scene);
            if (!direction)
                continue;
            const auto enu = icrs_to_enu(*direction, observer, rotation);
            const auto projected =
                project_zenith_perspective(enu, 1280.0 / 720.0);
            if (!projected)
                continue;
            // Owner-approved temporary Toulouse M1 cutoff, after visibility
            // calculation.
            if (star.magnitude().gaia_g() > 4.0)
            {
                ++scene.magnitude_excluded;
                continue;
            }
            scene.points.push_back(
                ScreenPoint{static_cast<float>(projected->x),
                            static_cast<float>(projected->y),
                            m1_display_intensity(star.magnitude().gaia_g())});
        }
        return scene;
    }
} // namespace msc::cli
