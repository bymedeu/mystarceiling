#include <mystarceiling/catalogue/select_brightest.hh>

#include <algorithm>
#include <cstddef>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <tuple>
#include <vector>

namespace msc::catalogue
{
    std::vector<CatalogueStar>
    select_brightest(std::vector<CatalogueStar> stars, std::size_t maximum)
    {
        std::stable_sort(
            stars.begin(), stars.end(),
            [](const CatalogueStar& a, const CatalogueStar& b)
            {
                return std::tuple{a.star().magnitude().gaia_g(),
                                  a.source_catalogue(), a.source_id()} <
                    std::tuple{b.star().magnitude().gaia_g(),
                               b.source_catalogue(), b.source_id()};
            });
        if (stars.size() > maximum)
            stars.erase(stars.begin() + maximum, stars.end());
        return stars;
    }
} // namespace msc::catalogue
