#include <gtest/gtest.h>

#include "cli/m1_scene.hh"

#include <array>
#include <cstdint>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/projection/zenith_perspective.hh>
#include <mystarceiling/core/time/reference_epoch.hh>

using msc::catalogue::CatalogueSource;
using msc::catalogue::CatalogueStar;
using msc::cli::make_m1_scene;
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
    CatalogueStar star(double magnitude, double dec = 33.16601472318802,
                       CatalogueSource source = CatalogueSource::GaiaDr3,
                       double epoch = 2016, double motion = 0)
    {
        return CatalogueStar{
            1, source,
            Star{
                *ReferenceEpoch::try_from_julian_year(epoch),
                IcrsDirection{*RightAscension::try_from(
                                  Angle::from_degrees(74.24843702624405)),
                              *Declination::try_from(Angle::from_degrees(dec))},
                ProperMotionComponents{
                    *ProperMotion::try_from_milliarcseconds_per_year(motion),
                    *ProperMotion::try_from_milliarcseconds_per_year(0)},
                *Parallax::try_from_milliarcseconds(1),
                *ApparentMagnitude::try_from_gaia_g(magnitude),
                msc::core::astronomy::LinearRgb8{255, 255, 255}}};
    }
} // namespace

TEST(M1SceneTest, RetainsAboveHorizonSourcesWithFixedDirection)
{
    const auto scene =
        make_m1_scene(std::array{star(4.0), star(4.000001), star(5, -80)});
    ASSERT_EQ(scene.stars.size(), 2);
    EXPECT_DOUBLE_EQ(scene.stars[0].magnitude, 4.0);
    EXPECT_DOUBLE_EQ(scene.stars[1].magnitude, 4.000001);
    // Known Gaia direction, unchanged by the application scene builder.
    const auto projected = msc::core::projection::project_zenith_perspective(
        scene.stars[0].enu, 1280.0 / 720.0);
    ASSERT_TRUE(projected);
    EXPECT_NEAR(projected->x, -0.23376923971854943, 1e-12);
    EXPECT_NEAR(projected->y, -0.12855289148878088, 1e-12);
}

TEST(M1SceneTest, DoesNotRepeatMotionForEitherPreparedSource)
{
    const auto scene = make_m1_scene(std::array{
        star(2, 33, CatalogueSource::Hipparcos2, 2016, 3600000),
        star(2, 33, CatalogueSource::GaiaDr3, 2016, 3600000), star(2, 33)});
    ASSERT_EQ(scene.stars.size(), 3);
    for (const auto& star : scene.stars)
    {
        EXPECT_EQ(star.enu.x(), scene.stars.front().enu.x());
        EXPECT_EQ(star.enu.y(), scene.stars.front().enu.y());
        EXPECT_EQ(star.enu.z(), scene.stars.front().enu.z());
    }
}

TEST(M1SceneTest, PreservesPreparedPackedColourAndMagnitude)
{
    const auto white = star(2.1975422);
    const auto& baseline = white.star();
    const CatalogueStar coloured{
        2, CatalogueSource::GaiaDr3,
        Star{baseline.reference_epoch(), baseline.direction(),
             baseline.proper_motion(), baseline.parallax(),
             baseline.magnitude(),
             msc::core::astronomy::LinearRgb8{255, 17, 0}}};
    const auto scene = make_m1_scene(std::array{white, coloured});
    ASSERT_EQ(scene.stars.size(), 2);
    EXPECT_EQ(scene.stars[1].colour,
              (msc::core::astronomy::LinearRgb8{255, 17, 0}));
    EXPECT_DOUBLE_EQ(scene.stars[0].magnitude, baseline.magnitude().gaia_g());
    EXPECT_DOUBLE_EQ(scene.stars[1].magnitude, baseline.magnitude().gaia_g());
}

TEST(M1SceneTest, RetainsFaintSourcesForRendererSelection)
{
    const auto scene = make_m1_scene(std::array{star(2), star(9)});
    ASSERT_EQ(scene.stars.size(), 2);
    EXPECT_EQ(scene.stars[0].magnitude, 2);
    EXPECT_EQ(scene.stars[1].magnitude, 9);
    EXPECT_GT(scene.stars[0].enu.z(), 0);
    EXPECT_DOUBLE_EQ(scene.settings.limiting_magnitude, 6);
}

TEST(M1SceneTest, AppearanceKeysRetainHighIdBitsAndCatalogueIdentity)
{
    const auto base = star(2);
    const std::array stars{
        CatalogueStar{1, CatalogueSource::GaiaDr3, base.star()},
        CatalogueStar{(std::uint64_t{1} << 32) | 1, CatalogueSource::GaiaDr3,
                      base.star()},
        CatalogueStar{1, CatalogueSource::Hipparcos2, base.star()}};
    const auto scene = make_m1_scene(stars);
    ASSERT_EQ(scene.stars.size(), 3);
    const auto first = scene.stars[0].animation_key;
    EXPECT_EQ(first.low, scene.stars[1].animation_key.low);
    EXPECT_NE(first.high, scene.stars[1].animation_key.high);
    EXPECT_NE(first.high, scene.stars[2].animation_key.high);
}
