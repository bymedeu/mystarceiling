#pragma once

#include <cstddef>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <vector>

namespace msc::catalogue
{
    // Consumes normalized records. Equal magnitudes order by catalogue, then
    // native ID; identical keys retain input order. Source files are never
    // modified.
    [[nodiscard]] std::vector<CatalogueStar>
    select_brightest(std::vector<CatalogueStar> stars, std::size_t maximum);
} // namespace msc::catalogue
