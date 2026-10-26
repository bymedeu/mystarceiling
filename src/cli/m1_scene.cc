#include "cli/m1_scene.hh"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/appearance/scintillation.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/projection/zenith_perspective.hh>
#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>
#include <mystarceiling/render/opengl_renderer.hh>
#include <optional>
#include <span>

namespace msc::cli
{
    using catalogue::CatalogueStar;
    using core::coordinates::icrs_to_enu;
    using core::coordinates::Latitude;
    using core::coordinates::Longitude;
    using core::coordinates::ObserverLocation;
    using core::maths::Angle;
    using core::projection::project_zenith_perspective;
    using core::time::earth_rotation_angle;
    using core::time::JulianDate;
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
        float linear_intensity(double magnitude) noexcept
        {
            // Preserve the existing white-star display curve; colour scales
            // linear light before the renderer applies its output encoding.
            const auto display =
                static_cast<double>(m1_display_intensity(magnitude));
            return static_cast<float>(
                display <= 0.04045 ? display / 12.92
                                   : std::pow((display + 0.055) / 1.055, 2.4));
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
        M1Scene scene;
        scene.points.reserve(stars.size());
        for (const auto& entry : stars)
        {
            const auto& star = entry.star();
            const auto enu = icrs_to_enu(star.direction(), observer, rotation);
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
            scene.points.push_back(ScreenPoint{
                static_cast<float>(projected->x),
                static_cast<float>(projected->y),
                linear_intensity(star.magnitude().gaia_g()), star.colour()});
        }
        return scene;
    }

    M1Scene make_appearance_scene(std::span<const CatalogueStar> stars,
                                  const render::TrialSettings& settings)
    {
        const ObserverLocation observer{
            *Latitude::try_from(Angle::from_degrees(43.6045)),
            *Longitude::try_from(Angle::from_degrees(1.4440))};
        const auto rotation = earth_rotation_angle(JulianDate{2457388.5, 0.0});
        M1Scene scene;
        scene.trial = settings;
        scene.trial_stars.reserve(stars.size());
        for (const auto& entry : stars)
        {
            const auto& star = entry.star();
            const auto enu = icrs_to_enu(star.direction(), observer, rotation);
            if (enu.z() <= 0)
                continue;
            const auto id = entry.source_id();
            const auto catalogue =
                static_cast<std::uint32_t>(entry.source_catalogue());
            const core::appearance::AnimationKey key{
                static_cast<std::uint32_t>(id),
                static_cast<std::uint32_t>(id >> 32) ^
                    (0x9e3779b9U * catalogue)};
            scene.trial_stars.push_back(
                {enu, star.magnitude().gaia_g(), star.colour(), key});
        }
        return scene;
    }
} // namespace msc::cli
