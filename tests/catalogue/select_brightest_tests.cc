#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mystarceiling/catalogue/select_brightest.hh>
#include <mystarceiling/catalogue/star.hh>
#include <mystarceiling/catalogue/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <optional>
#include <vector>

using msc::catalogue::ApparentMagnitude;
using msc::catalogue::CatalogueSource;
using msc::catalogue::Parallax;
using msc::catalogue::select_brightest;
using msc::catalogue::Star;
using msc::core::coordinates::Declination;
using msc::core::coordinates::IcrsDirection;
using msc::core::coordinates::ProperMotion;
using msc::core::coordinates::ProperMotionComponents;
using msc::core::coordinates::RightAscension;
using msc::core::maths::Angle;
using msc::core::time::ReferenceEpoch;

namespace
{
    Star star(std::uint64_t id, CatalogueSource source, double magnitude)
    {
        const auto zero = *ProperMotion::try_from_milliarcseconds_per_year(0);
        return Star{
            id,
            *ReferenceEpoch::try_from_julian_year(2016),
            IcrsDirection{*RightAscension::try_from(Angle::from_degrees(0)),
                          *Declination::try_from(Angle::from_degrees(0))},
            ProperMotionComponents{zero, zero},
            *Parallax::try_from_milliarcseconds(1),
            *ApparentMagnitude::try_from_gaia_g(magnitude),
            std::nullopt,
            source};
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
    std::vector<Star> input;
    for (std::uint64_t id = 1; id <= 4096; ++id)
        input.push_back(star(id, CatalogueSource::GaiaDr3, 2 + id / 4096.0));
    for (std::uint64_t id = 1; id <= 62; ++id)
        input.push_back(star(id, CatalogueSource::Hipparcos2, 1));
    const auto selected = select_brightest(input, 4096);
    ASSERT_EQ(selected.size(), 4096);
    EXPECT_EQ(
        std::count_if(
            selected.begin(), selected.end(), [](const Star& s)
            { return s.source_catalogue() == CatalogueSource::Hipparcos2; }),
        62);
    EXPECT_EQ(selected.back().source_id(), 4034);
    EXPECT_EQ(input.size(), 4158);
}
