#pragma once

#include <cstdint>
#include <expected>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/coordinates/propagate_proper_motion.hh>
#include <mystarceiling/core/time/reference_epoch.hh>

namespace msc::catalogue
{
    inline std::expected<CatalogueStar,
                         core::coordinates::ProperMotionPropagationError>
    prepare_star(std::uint64_t source_id, CatalogueSource source,
                 const core::astronomy::Star& star) noexcept
    {
        const auto epoch =
            *core::time::ReferenceEpoch::try_from_julian_year(2016.0);
        const auto direction = core::coordinates::propagate_proper_motion(
            star.direction(), star.proper_motion(), star.reference_epoch(),
            epoch);
        if (!direction)
            return std::unexpected{direction.error()};
        // M2 approximation: retain measured motion components and parallax.
        // These are not a fully rebased six-dimensional astrometric solution.
        return CatalogueStar{
            source_id, source,
            core::astronomy::Star{epoch, *direction, star.proper_motion(),
                                  star.parallax(), star.magnitude(),
                                  star.colour()}};
    }
} // namespace msc::catalogue
