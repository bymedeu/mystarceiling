#pragma once

#include <cstddef>
#include <mystarceiling/catalogue/star.hh>
#include <vector>

namespace msc::catalogue
{
    // Consumes normalized records. Equal magnitudes order by catalogue, then
    // native ID; identical keys retain input order. Source files are never
    // modified.
    [[nodiscard]] std::vector<Star> select_brightest(std::vector<Star> stars,
                                                     std::size_t maximum);
} // namespace msc::catalogue
