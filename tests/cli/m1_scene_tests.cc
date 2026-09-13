#include <gtest/gtest.h>

#include "cli/m1_scene.hh"

#include <array>
#include <limits>
#include <mystarceiling/catalogue/catalogue_star.hh>
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
                *ApparentMagnitude::try_from_gaia_g(magnitude), std::nullopt}};
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

TEST(M1SceneTest, HipparcosMovesToJ2016AndGaiaDirectionIsUnchanged)
{
    const auto scene = make_m1_scene(std::array{
        star(2, 33, CatalogueSource::Hipparcos2, 1991.25, 3600000),
        star(2, 33, CatalogueSource::GaiaDr3, 2016, 3600000), star(2, 33)});
    ASSERT_EQ(scene.points.size(), 3);
    // Large eastward motion provides an unmistakable screen-space signal;
    // core tests separately validate the tangent-vector numerical model.
    EXPECT_GT(scene.points[0].x, scene.points[1].x + 0.2F);
    EXPECT_EQ(scene.points[1].x, scene.points[2].x);
    EXPECT_EQ(scene.points[1].y, scene.points[2].y);
}

TEST(M1SceneTest, CountsAndSkipsFailedPropagationWithoutUsingStaleDirection)
{
    const auto scene =
        make_m1_scene(std::array{star(2, 33, CatalogueSource::Hipparcos2,
                                      -std::numeric_limits<double>::max(),
                                      std::numeric_limits<double>::max()),
                                 star(2)});
    EXPECT_EQ(scene.propagation_failures, 1);
    EXPECT_EQ(scene.magnitude_excluded, 0);
    EXPECT_EQ(scene.points.size(), 1);
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
