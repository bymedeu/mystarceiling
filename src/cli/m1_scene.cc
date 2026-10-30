#include "cli/m1_scene.hh"

#include <cstdint>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/appearance/scintillation.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>
#include <span>

namespace msc::cli
{
    using catalogue::CatalogueStar;
    using core::coordinates::icrs_to_enu;
    using core::coordinates::Latitude;
    using core::coordinates::Longitude;
    using core::coordinates::ObserverLocation;
    using core::maths::Angle;
    using core::time::earth_rotation_angle;
    using core::time::JulianDate;

    M1Scene make_m1_scene(std::span<const CatalogueStar> stars)
    {
        const ObserverLocation observer{
            *Latitude::try_from(Angle::from_degrees(43.6045)),
            *Longitude::try_from(Angle::from_degrees(1.4440))};
        const auto rotation = earth_rotation_angle(JulianDate{2457388.5, 0.0});
        M1Scene scene;
        scene.stars.reserve(stars.size());
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
            scene.stars.push_back(
                {enu, star.magnitude().gaia_g(), star.colour(), key});
        }
        return scene;
    }
} // namespace msc::cli
