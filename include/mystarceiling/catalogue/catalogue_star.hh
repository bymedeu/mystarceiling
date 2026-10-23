#pragma once

#include <cstdint>
#include <mystarceiling/core/astronomy/star.hh>

namespace msc::catalogue
{
    using core::astronomy::Star;

    // Values are also the version-2 binary provenance encoding, not ID bits.
    enum class CatalogueSource : std::uint8_t
    {
        GaiaDr3 = 0,
        Hipparcos2 = 1
    };

    class CatalogueStar
    {
    public:
        constexpr CatalogueStar(std::uint64_t source_id,
                                CatalogueSource source_catalogue,
                                const Star& star) noexcept :
            source_id_{source_id}, source_catalogue_{source_catalogue},
            star_{star}
        {}

        [[nodiscard]] constexpr std::uint64_t source_id() const noexcept
        {
            return source_id_;
        }
        [[nodiscard]] constexpr CatalogueSource
        source_catalogue() const noexcept
        {
            return source_catalogue_;
        }
        [[nodiscard]] constexpr const Star& star() const noexcept
        {
            return star_;
        }

    private:
        std::uint64_t source_id_;
        CatalogueSource source_catalogue_;
        Star star_;
    };
} // namespace msc::catalogue
