#include <mystarceiling/catalogue/select_brightest.hh>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace msc::catalogue
{
    bool brightness_less(const CatalogueStar& a, const CatalogueStar& b)
    {
        return std::tuple{a.star().magnitude().gaia_g(), a.source_catalogue(),
                          a.source_id()} <
            std::tuple{b.star().magnitude().gaia_g(), b.source_catalogue(),
                       b.source_id()};
    }

    namespace
    {
        struct Resolution
        {
            std::vector<CatalogueStar> required;
            std::vector<std::pair<std::size_t, CatalogueStar>> deferred;
        };

        Resolution classify_links(
            std::vector<CatalogueStar> hipparcos,
            const std::unordered_map<std::uint64_t, std::uint64_t>& preferences,
            const std::vector<CatalogueStar>& gaia)
        {
            std::unordered_map<std::uint64_t, std::size_t> ranks;
            for (std::size_t i = 0; i < gaia.size(); ++i)
                ranks.emplace(gaia[i].source_id(), i);
            Resolution result;
            auto& [required, deferred] = result;
            for (auto& star : hipparcos)
            {
                const auto link = preferences.find(star.source_id());
                if (link == preferences.end())
                    required.push_back(std::move(star));
                else
                {
                    const auto rank = ranks.find(link->second);
                    deferred.emplace_back(
                        rank == ranks.end()
                            ? std::numeric_limits<std::size_t>::max()
                            : rank->second,
                        std::move(star));
                }
            }
            return result;
        }
    } // namespace

    std::vector<CatalogueStar> resolve_hipparcos_first(
        std::vector<CatalogueStar> hipparcos,
        const std::unordered_map<std::uint64_t, std::uint64_t>& preferences,
        std::vector<CatalogueStar> gaia, std::size_t capacity)
    {
        auto [required, deferred] =
            classify_links(std::move(hipparcos), preferences, gaia);
        std::ranges::sort(deferred, std::greater{},
                          &decltype(deferred)::value_type::first);
        for (auto& [rank, star] : deferred)
        {
            const auto remaining =
                capacity - std::min(capacity, required.size());
            if (rank < remaining)
                break;
            required.push_back(std::move(star));
        }
        required = select_brightest(std::move(required), capacity);
        const auto remaining =
            std::min(capacity - required.size(), gaia.size());
        required.insert(required.end(), std::make_move_iterator(gaia.begin()),
                        std::make_move_iterator(gaia.begin() + remaining));
        return select_brightest(std::move(required), capacity);
    }

    std::vector<CatalogueStar>
    select_brightest(std::vector<CatalogueStar> stars, std::size_t maximum)
    {
        std::stable_sort(stars.begin(), stars.end(), brightness_less);
        if (stars.size() > maximum)
            stars.erase(stars.begin() + maximum, stars.end());
        return stars;
    }
} // namespace msc::catalogue
