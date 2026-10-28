#include "cli/config.hh"
#include "cli/launch.hh"
#include "cli/m1_scene.hh"
#include "launch_test_support.hh"

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <gtest/gtest.h>
#include <mystarceiling/catalogue/binary_catalogue.hh>
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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using msc::cli::test::LaunchTest;

namespace
{
    void write_catalogue(const std::filesystem::path& path)
    {
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
        const Star star{
            *ReferenceEpoch::try_from_julian_year(2016.0),
            IcrsDirection{*RightAscension::try_from(Angle::from_degrees(10.5)),
                          *Declination::try_from(Angle::from_degrees(45.0))},
            ProperMotionComponents{
                *ProperMotion::try_from_milliarcseconds_per_year(0),
                *ProperMotion::try_from_milliarcseconds_per_year(0)},
            *Parallax::try_from_milliarcseconds(1),
            *ApparentMagnitude::try_from_gaia_g(3),
            msc::core::astronomy::LinearRgb8{255, 255, 255}};
        std::vector<msc::catalogue::CatalogueStar> stars;
        for (std::uint64_t id = 1; id <= 64; ++id)
            stars.emplace_back(id, msc::catalogue::CatalogueSource::GaiaDr3,
                               star);
        ASSERT_TRUE(msc::catalogue::write_binary_catalogue(path, stars));
    }
} // namespace

TEST_F(LaunchTest, SmallPreparedBinaryBuildsUnchangedSceneWithoutRawSources)
{
    write_catalogue(root / "catalogue.bin");
    msc::cli::Settings settings{root / "catalogue.bin", 64, {}, "off"};
    const auto scene = msc::cli::prepare_view(settings);
    const auto stars =
        msc::catalogue::read_binary_catalogue(settings.catalogue, 64);
    ASSERT_TRUE(stars);
    const auto expected = msc::cli::make_m1_scene(*stars);
    ASSERT_EQ(scene.stars.size(), expected.stars.size());
    ASSERT_EQ(scene.stars.size(), 64);
    EXPECT_DOUBLE_EQ(scene.stars.front().magnitude, 3);
    EXPECT_EQ(scene.stars.front().colour, stars->front().star().colour());
    EXPECT_DOUBLE_EQ(scene.settings.limiting_magnitude, 6);
    EXPECT_FALSE(std::filesystem::exists(root / "sources"));
}

TEST_F(LaunchTest, SavedExcessiveQuantityCanBeOverriddenWithoutStdoutOutput)
{
    write(config(), valid_config());
    write_catalogue(root / "catalogue.bin");
    const std::array args{"msc",         "view", "--catalogue", "catalogue.bin",
                          "--quantity",  "64",   "--log-file",  "",
                          "--log-level", "off"};
    std::ostringstream output;
    std::ostringstream errors;
    bool shown = false;
    EXPECT_EQ(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                [&](const msc::cli::M1Scene&)
                                { shown = true; }),
              0);
    EXPECT_TRUE(shown);
    EXPECT_TRUE(errors.str().empty());
    EXPECT_TRUE(output.str().empty());
    EXPECT_EQ(read(config()), valid_config());
}

TEST_F(LaunchTest, SceneSummaryIsLoggedToFileAtInfoLevelWithoutStdoutOutput)
{
    write(config(), valid_config());
    write_catalogue(root / "catalogue.bin");
    const std::array args{"msc",         "view", "--catalogue", "catalogue.bin",
                          "--quantity",  "64",   "--log-file",  "scene.log",
                          "--log-level", "info"};
    std::ostringstream output;
    std::ostringstream errors;
    std::string summary;
    EXPECT_EQ(msc::cli::run_cli(
                  args.size(), args.data(), output, errors,
                  [&](const msc::cli::M1Scene& scene)
                  {
                      summary =
                          "MSC viewer: " + std::to_string(scene.stars.size()) +
                          " above-horizon sources; G-like limit 6 applied by "
                          "the renderer.\n";
                  }),
              0);
    EXPECT_TRUE(output.str().empty());
    EXPECT_TRUE(errors.str().empty());
    ASSERT_FALSE(summary.empty());
    const auto log = read(root / "scene.log");
    EXPECT_NE(log.find("[info]"), std::string::npos);
    EXPECT_NE(log.find(summary), std::string::npos);
}

TEST_F(LaunchTest, ReaderRejectsEffectiveCountOutsideAvailableRange)
{
    write_catalogue(root / "catalogue.bin");
    for (const std::size_t count : {0, 63, 65, 4096})
    {
        const msc::cli::Settings settings{
            root / "catalogue.bin", count, {}, "off"};
        try
        {
            (void)msc::cli::prepare_view(settings);
            FAIL() << count;
        }
        catch (const std::runtime_error& error)
        {
            EXPECT_NE(std::string{error.what()}.find("Requested " +
                                                     std::to_string(count) +
                                                     " records; available 64"),
                      std::string::npos);
        }
    }
}

TEST_F(LaunchTest, HelpAndVersionBypassInvalidConfigAndAllLaunchSideEffects)
{
    const auto bad = root / "bad.toml";
    const auto log = (root / "keep.log").string();
    write(bad, "this is invalid TOML");
    write(log, "keep log");
    const auto bad_string = bad.string();
    const std::vector<std::vector<const char*>> cases{
        {"msc"},
        {"msc", "--help"},
        {"msc", "view", "--help"},
        {"msc", "catalogue", "--help"},
        {"msc", "catalogue", "build", "--help"},
        {"msc", "catalogue", "info", "--help"},
        {"msc", "--version"},
        {"msc", "--config", bad_string.c_str(), "--log-file", log.c_str(),
         "--help"},
        {"msc", "view", "--config", bad_string.c_str(), "--version"}};
    for (const auto& args : cases)
    {
        std::ostringstream output;
        std::ostringstream errors;
        EXPECT_EQ(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                    [](const msc::cli::M1Scene&) { FAIL(); }),
                  0);
        EXPECT_FALSE(output.str().empty());
        EXPECT_TRUE(errors.str().empty());
        EXPECT_FALSE(std::filesystem::exists(config()));
        EXPECT_EQ(read(log), "keep log");
        EXPECT_EQ(read(bad), "this is invalid TOML");
    }
}

TEST_F(LaunchTest, UnsupportedCommandsAndSourceOptionsFailBeforeConfigCreation)
{
    const std::vector<std::vector<const char*>> cases{
        {"msc", "catalogue", "unknown"},
        {"msc", "old.csv", "old2.csv", "out.bin"},
        {"msc", "view", "--appearance-trial"},
        {"msc", "view", "--animation-trial"},
        {"msc", "view", "--gaia", "source.csv"},
        {"msc", "view", "--hipparcos", "source.csv"}};
    for (const auto& args : cases)
    {
        std::ostringstream output;
        std::ostringstream errors;
        EXPECT_NE(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                    [](const msc::cli::M1Scene&) { FAIL(); }),
                  0);
        EXPECT_FALSE(errors.str().empty());
        EXPECT_FALSE(std::filesystem::exists(config()));
    }
}

TEST_F(LaunchTest, LogOpenFailureIsReportedToStderrAndStopsBeforeScene)
{
    write(config(), valid_config());
    const auto directory = root.string();
    const std::array args{"msc", "--log-file", directory.c_str(), "view"};
    std::ostringstream output;
    std::ostringstream errors;
    EXPECT_NE(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                [](const msc::cli::M1Scene&) { FAIL(); }),
              0);
    EXPECT_TRUE(output.str().empty());
    EXPECT_NE(errors.str().find("Failed opening file"), std::string::npos);
}

TEST_F(LaunchTest, FirstLaunchUsesCliOverridesButPersistsOnlyInitialDefaults)
{
    write_catalogue(root / "catalogue.bin");
    const auto log = (root / "isolated.log").string();
    const std::array args{"msc",         "--log-file",    log.c_str(),  "view",
                          "--catalogue", "catalogue.bin", "--quantity", "64"};
    std::ostringstream output;
    std::ostringstream errors;
    bool shown = false;
    EXPECT_EQ(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                [&](const msc::cli::M1Scene&)
                                { shown = true; }),
              0);
    EXPECT_TRUE(shown);
    EXPECT_TRUE(errors.str().empty());
    EXPECT_TRUE(output.str().empty());
    EXPECT_TRUE(std::filesystem::exists(log));
    EXPECT_TRUE(read(log).empty());
    const auto saved = msc::cli::load_settings({});
    EXPECT_EQ(saved.log_level, "warning");
    EXPECT_EQ(saved.quantity, 4096);
    EXPECT_EQ(saved.log_file, "/tmp/mystarceiling.log");
    EXPECT_EQ(saved.catalogue, root / "data/mystarceiling/catalogue.bin");
}

TEST_F(LaunchTest, PlainViewReachesAnimatedViewerWithoutEditingConfig)
{
    auto saved = valid_config();
    const std::string quantity = "quantity = 4096";
    saved.replace(saved.find(quantity), quantity.size(), "quantity = 64");
    write(config(), saved);
    write_catalogue(config().parent_path() / "catalogue.bin");
    const std::array args{"msc", "view"};
    std::ostringstream output, errors;
    bool shown = false;
    EXPECT_EQ(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                [&](const msc::cli::M1Scene& scene)
                                {
                                    shown = true;
                                    EXPECT_FALSE(scene.settings.picture_mode);
                                    EXPECT_DOUBLE_EQ(
                                        scene.settings.limiting_magnitude, 6);
                                    ASSERT_EQ(scene.stars.size(), 64);
                                    EXPECT_GT(scene.stars.front().enu.z(), 0);
                                }),
              0);
    EXPECT_TRUE(shown);
    EXPECT_TRUE(errors.str().empty());
    EXPECT_EQ(read(config()), saved);
}

TEST_F(LaunchTest, RemovedViewerFlagsFailBeforeConfigAndLaunchSideEffects)
{
    const auto bad = root / "bad.toml";
    const auto log = root / "keep.log";
    const auto binary = root / "keep.bin";
    write(bad, "invalid TOML");
    write(log, "keep log");
    write(binary, "keep catalogue");
    const auto bad_string = bad.string();
    const auto log_string = log.string();
    const auto binary_string = binary.string();
    for (const auto* flag : {"--appearance-trial", "--animation-trial"})
    {
        const std::array args{"msc",
                              "view",
                              flag,
                              "--config",
                              bad_string.c_str(),
                              "--log-file",
                              log_string.c_str(),
                              "--catalogue",
                              binary_string.c_str()};
        std::ostringstream output, errors;
        EXPECT_NE(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                    [](const msc::cli::M1Scene&) { FAIL(); }),
                  0);
        EXPECT_NE(errors.str().find(flag), std::string::npos);
        EXPECT_TRUE(output.str().empty());
        EXPECT_FALSE(std::filesystem::exists(config()));
        EXPECT_EQ(read(bad), "invalid TOML");
        EXPECT_EQ(read(log), "keep log");
        EXPECT_EQ(read(binary), "keep catalogue");
    }
}

TEST_F(LaunchTest, RemovedTuningOptionsFailIndividuallyBeforeLaunchSideEffects)
{
    const std::vector<std::vector<const char*>> cases{
        {"msc", "view", "--brightness", "25"},
        {"msc", "view", "--ray-fraction", "0.4"},
        {"msc", "view", "--twinkle-strength", "0.5"},
        {"msc", "view", "--width", "1280"},
        {"msc", "view", "--seed", "123"},
        {"msc", "view", "--picture-mode"}};
    for (const auto& args : cases)
    {
        std::ostringstream output, errors;
        EXPECT_NE(msc::cli::run_cli(args.size(), args.data(), output, errors,
                                    [](const msc::cli::M1Scene&) { FAIL(); }),
                  0);
        EXPECT_FALSE(errors.str().empty());
        EXPECT_FALSE(std::filesystem::exists(config()));
    }
}
