#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/catalogue/select_brightest.hh>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <optional>
#include <unordered_map>
#include <vector>

using msc::catalogue::CatalogueSource;
using msc::catalogue::CatalogueStar;
using msc::catalogue::select_brightest;
using msc::core::astronomy::ApparentMagnitude;
using msc::core::astronomy::Parallax;
using msc::core::astronomy::Star;
using msc::core::coordinates::Declination;
using msc::core::coordinates::IcrsDirection;
using msc::core::coordinates::ProperMotion;
using msc::core::coordinates::ProperMotionComponents;
using msc::core::coordinates::RightAscension;
using msc::core::maths::Angle;
using msc::core::time::ReferenceEpoch;

namespace
{
    CatalogueStar star(std::uint64_t id, CatalogueSource source,
                       double magnitude)
    {
        const auto zero = *ProperMotion::try_from_milliarcseconds_per_year(0);
        return CatalogueStar{
            id, source,
            Star{
                *ReferenceEpoch::try_from_julian_year(2016),
                IcrsDirection{*RightAscension::try_from(Angle::from_degrees(0)),
                              *Declination::try_from(Angle::from_degrees(0))},
                ProperMotionComponents{zero, zero},
                *Parallax::try_from_milliarcseconds(1),
                *ApparentMagnitude::try_from_gaia_g(magnitude),
                msc::core::astronomy::LinearRgb8{255, 255, 255}}};
    }
} // namespace

TEST(SelectBrightestTest, OrdersTiesBySeparateProvenanceAndFullNativeId)
{
    const auto largest_id = std::numeric_limits<std::uint64_t>::max();
    std::vector input{star(7, CatalogueSource::Hipparcos2, 2),
                      star(largest_id, CatalogueSource::GaiaDr3, 2),
                      star(7, CatalogueSource::GaiaDr3, 2),
                      star(9, CatalogueSource::Hipparcos2, -1)};
    const auto first = select_brightest(input, 4);
    std::reverse(input.begin(), input.end());
    const auto second = select_brightest(input, 4);
    ASSERT_EQ(first.size(), 4);
    EXPECT_EQ(first[0].source_id(), 9);
    EXPECT_EQ(first[1].source_id(), 7);
    EXPECT_EQ(first[1].source_catalogue(), CatalogueSource::GaiaDr3);
    EXPECT_EQ(first[2].source_id(), largest_id);
    EXPECT_EQ(first[3].source_catalogue(), CatalogueSource::Hipparcos2);
    for (std::size_t i = 0; i < first.size(); ++i)
    {
        EXPECT_EQ(first[i].source_id(), second[i].source_id());
        EXPECT_EQ(first[i].source_catalogue(), second[i].source_catalogue());
    }
    EXPECT_TRUE(select_brightest(input, 0).empty());
}

TEST(SelectBrightestTest, RetainsAllBrighterSupplementRecordsWithin4096Cap)
{
    std::vector<CatalogueStar> input;
    for (std::uint64_t id = 1; id <= 4096; ++id)
        input.push_back(star(id, CatalogueSource::GaiaDr3, 2 + id / 4096.0));
    for (std::uint64_t id = 1; id <= 62; ++id)
        input.push_back(star(id, CatalogueSource::Hipparcos2, 1));
    const auto selected = select_brightest(input, 4096);
    ASSERT_EQ(selected.size(), 4096);
    EXPECT_EQ(
        std::count_if(
            selected.begin(), selected.end(), [](const CatalogueStar& s)
            { return s.source_catalogue() == CatalogueSource::Hipparcos2; }),
        62);
    EXPECT_EQ(selected.back().source_id(), 4034);
    EXPECT_EQ(input.size(), 4158);
}

TEST(SelectBrightestTest,
     ResolvesGaiaPreferenceAgainstFinalPrefixWithoutDuplicates)
{
    const std::vector hipparcos{star(14576, CatalogueSource::Hipparcos2, 2.07),
                                star(25930, CatalogueSource::Hipparcos2, 2.18),
                                star(58001, CatalogueSource::Hipparcos2, 2.39)};
    const std::vector gaia{star(1, CatalogueSource::GaiaDr3, 1),
                           star(2, CatalogueSource::GaiaDr3, 2),
                           star(3, CatalogueSource::GaiaDr3, 3)};
    // Rank 2 falls outside the two slots remaining after Algol's fallback.
    // Its fallback then pushes rank 1 out too: both must survive as Hipparcos.
    const std::unordered_map<std::uint64_t, std::uint64_t> links{{25930, 3},
                                                                 {58001, 2}};
    const auto small =
        msc::catalogue::resolve_hipparcos_first(hipparcos, links, gaia, 3);
    ASSERT_EQ(small.size(), 3);
    for (const auto& record : small)
        EXPECT_EQ(record.source_catalogue(), CatalogueSource::Hipparcos2);
    const auto large =
        msc::catalogue::resolve_hipparcos_first(hipparcos, links, gaia, 4);
    ASSERT_EQ(large.size(), 4);
    EXPECT_EQ(large.front().source_id(), 1);
    EXPECT_EQ(std::ranges::count(large, CatalogueSource::Hipparcos2,
                                 &CatalogueStar::source_catalogue),
              1);
    EXPECT_TRUE(std::ranges::is_sorted(large, msc::catalogue::brightness_less));
}

TEST(SelectBrightestTest,
     MissingGaiaAndTinyCapacityRetainBrightestNativeFallbacks)
{
    std::vector input{star(58001, CatalogueSource::Hipparcos2, 2.39),
                      star(25930, CatalogueSource::Hipparcos2, 2.18),
                      star(14576, CatalogueSource::Hipparcos2, 2.07)};
    const std::unordered_map<std::uint64_t, std::uint64_t> links{{25930, 99}};
    const auto first =
        msc::catalogue::resolve_hipparcos_first(input, links, {}, 2);
    std::reverse(input.begin(), input.end());
    const auto second =
        msc::catalogue::resolve_hipparcos_first(input, links, {}, 2);
    ASSERT_EQ(first.size(), 2);
    ASSERT_EQ(second.size(), 2);
    EXPECT_EQ(first[0].source_id(), 14576);
    EXPECT_EQ(first[1].source_id(), 25930);
    for (std::size_t i = 0; i < first.size(); ++i)
        EXPECT_EQ(first[i].source_id(), second[i].source_id());
}
