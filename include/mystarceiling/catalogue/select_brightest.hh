#pragma once

#include <cstddef>
#include <cstdint>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <unordered_map>
#include <vector>

namespace msc::catalogue
{
    [[nodiscard]] bool brightness_less(const CatalogueStar& a,
                                       const CatalogueStar& b);

    // Resolve published Gaia preferences against the final accepted prefix.
    // Hipparcos fallbacks keep priority; capacity truncation remains explicit.
    [[nodiscard]] std::vector<CatalogueStar> resolve_hipparcos_first(
        std::vector<CatalogueStar> hipparcos,
        const std::unordered_map<std::uint64_t, std::uint64_t>& preferences,
        std::vector<CatalogueStar> gaia, std::size_t capacity);

    // Consumes normalized records. Equal magnitudes order by catalogue, then
    // native ID; identical keys retain input order. Source files are never
    // modified.
    [[nodiscard]] std::vector<CatalogueStar>
    select_brightest(std::vector<CatalogueStar> stars, std::size_t maximum);
} // namespace msc::catalogue
