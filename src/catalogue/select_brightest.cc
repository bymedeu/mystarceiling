#include <mystarceiling/catalogue/select_brightest.hh>

#include <algorithm>
#include <cstddef>
#include <mystarceiling/catalogue/star.hh>
#include <tuple>
#include <vector>

namespace msc::catalogue
{
    std::vector<Star> select_brightest(std::vector<Star> stars,
                                       std::size_t maximum)
    {
        std::stable_sort(
            stars.begin(), stars.end(),
            [](const Star& a, const Star& b)
            {
                return std::tuple{a.magnitude().gaia_g(), a.source_catalogue(),
                                  a.source_id()} <
                    std::tuple{b.magnitude().gaia_g(), b.source_catalogue(),
                               b.source_id()};
            });
        if (stars.size() > maximum)
            stars.erase(stars.begin() + maximum, stars.end());
        return stars;
    }
} // namespace msc::catalogue
