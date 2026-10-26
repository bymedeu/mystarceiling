#include <gtest/gtest.h>

#include "cli/m1_scene.hh"

#include <array>
#include <cstdint>
#include <limits>
#include <mystarceiling/catalogue/catalogue_star.hh>
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

using msc::catalogue::CatalogueSource;
using msc::catalogue::CatalogueStar;
using msc::cli::m1_display_intensity;
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

TEST(M1SceneTest, MagnitudeBoundaryIsInclusiveAndCountFollowsVisibility)
{
    const auto scene =
        make_m1_scene(std::array{star(4.0), star(4.000001), star(5, -80)});
    ASSERT_EQ(scene.points.size(), 1);
    EXPECT_EQ(scene.magnitude_excluded, 1);
    EXPECT_EQ(scene.propagation_failures, 0);
    // Known Gaia direction, unchanged by the application scene builder.
    EXPECT_NEAR(scene.points[0].x, -0.23376923971854943, 2e-8);
    EXPECT_NEAR(scene.points[0].y, -0.12855289148878088, 2e-8);
}

TEST(M1SceneTest, DoesNotRepeatMotionForEitherPreparedSource)
{
    const auto scene = make_m1_scene(std::array{
        star(2, 33, CatalogueSource::Hipparcos2, 2016, 3600000),
        star(2, 33, CatalogueSource::GaiaDr3, 2016, 3600000), star(2, 33)});
    ASSERT_EQ(scene.points.size(), 3);
    for (const auto& point : scene.points)
    {
        EXPECT_EQ(point.x, scene.points.front().x);
        EXPECT_EQ(point.y, scene.points.front().y);
    }
    EXPECT_EQ(scene.propagation_failures, 0);
}

TEST(M1SceneTest, BrightnessHasContrastThroughoutTemporaryVisibleRange)
{
    EXPECT_FLOAT_EQ(m1_display_intensity(-1.5), 1);
    EXPECT_GT(m1_display_intensity(-1), 8 * m1_display_intensity(4));
    EXPECT_GT(m1_display_intensity(2), m1_display_intensity(3));
    EXPECT_GT(m1_display_intensity(3), m1_display_intensity(4));
    EXPECT_GT(m1_display_intensity(4), 0.05F);
    EXPECT_FLOAT_EQ(m1_display_intensity(-10), 1);
}

TEST(M1SceneTest, CarriesPackedColourAndLinearBrightnessToPresentation)
{
    auto white = star(2);
    const auto& baseline = white.star();
    const CatalogueStar coloured{
        2, CatalogueSource::GaiaDr3,
        Star{baseline.reference_epoch(), baseline.direction(),
             baseline.proper_motion(), baseline.parallax(),
             baseline.magnitude(),
             msc::core::astronomy::LinearRgb8{255, 17, 0}}};
    const auto scene = make_m1_scene(std::array{white, coloured});
    ASSERT_EQ(scene.points.size(), 2);
    EXPECT_EQ(scene.points[1].colour,
              (msc::core::astronomy::LinearRgb8{255, 17, 0}));
    EXPECT_EQ(scene.points[0].intensity, scene.points[1].intensity);
    // Independent decode of the legacy G=2 white-star display value.
    EXPECT_NEAR(scene.points[0].intensity, 0.032957344424256865, 1e-8);
}

TEST(M1SceneTest, AppearanceTrialRetainsFaintSourcesForLiveMagnitudeChanges)
{
    const auto scene = msc::cli::make_appearance_scene(
        std::array{star(2), star(9)}, msc::render::TrialSettings{});
    ASSERT_TRUE(scene.trial);
    ASSERT_EQ(scene.trial_stars.size(), 2);
    EXPECT_EQ(scene.trial_stars[0].magnitude, 2);
    EXPECT_EQ(scene.trial_stars[1].magnitude, 9);
    EXPECT_GT(scene.trial_stars[0].enu.z(), 0);
    EXPECT_TRUE(scene.points.empty());
}

TEST(M1SceneTest, AppearanceKeysRetainHighIdBitsAndCatalogueIdentity)
{
    const auto base = star(2);
    const std::array stars{
        CatalogueStar{1, CatalogueSource::GaiaDr3, base.star()},
        CatalogueStar{(std::uint64_t{1} << 32) | 1, CatalogueSource::GaiaDr3,
                      base.star()},
        CatalogueStar{1, CatalogueSource::Hipparcos2, base.star()}};
    const auto scene = msc::cli::make_appearance_scene(stars, {});
    ASSERT_EQ(scene.trial_stars.size(), 3);
    const auto first = scene.trial_stars[0].animation_key;
    EXPECT_EQ(first.low, scene.trial_stars[1].animation_key.low);
    EXPECT_NE(first.high, scene.trial_stars[1].animation_key.high);
    EXPECT_NE(first.high, scene.trial_stars[2].animation_key.high);
}
