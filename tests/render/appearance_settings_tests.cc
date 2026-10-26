#include <gtest/gtest.h>

#include <limits>
#include <mystarceiling/render/appearance_trial.hh>

using msc::render::TrialSettings;
using msc::render::valid_trial_settings;

TEST(AppearanceSettingsTest, DefaultAndAllRayProfilesAreSupported)
{
    TrialSettings settings;
    EXPECT_TRUE(valid_trial_settings(settings));
    settings.halo_fraction = 0;
    settings.ray_fraction = 1;
    EXPECT_TRUE(valid_trial_settings(settings));
    settings.halo_fraction = 0.1;
    EXPECT_FALSE(valid_trial_settings(settings));
}

TEST(AppearanceSettingsTest, RejectsNonfiniteAndUnsupportedAllocationSizes)
{
    TrialSettings settings;
    settings.ray_length = std::numeric_limits<double>::infinity();
    EXPECT_FALSE(valid_trial_settings(settings));
    settings = {};
    settings.brightness = std::numeric_limits<double>::quiet_NaN();
    EXPECT_FALSE(valid_trial_settings(settings));
    settings = {};
    settings.width = 8192;
    EXPECT_FALSE(valid_trial_settings(settings));
    settings = {};
    settings.twinkle.strength = 1;
    EXPECT_FALSE(valid_trial_settings(settings));
}
