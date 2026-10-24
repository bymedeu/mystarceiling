#include "cli/config.hh"
#include "launch_test_support.hh"

#include <filesystem>
#include <gtest/gtest.h>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

using msc::cli::load_settings;
using msc::cli::Overrides;
using msc::cli::test::LaunchTest;

TEST_F(LaunchTest, MissingConfigStoresDefaultsBeforeOverrides)
{
    Overrides overrides;
    overrides.catalogue = "override.bin";
    overrides.quantity = "64";
    overrides.log_file = "";
    overrides.log_level = "off";
    const auto effective = load_settings(overrides);
    EXPECT_EQ(effective.catalogue, root / "override.bin");
    EXPECT_EQ(effective.quantity, 64);
    EXPECT_TRUE(effective.log_file.empty());
    EXPECT_EQ(effective.log_level, "off");
    const auto stored = load_settings({});
    EXPECT_EQ(stored.catalogue, root / "data/mystarceiling/catalogue.bin");
    EXPECT_EQ(stored.quantity, 4096);
    EXPECT_EQ(stored.log_file, "/tmp/mystarceiling.log");
    EXPECT_EQ(stored.log_level, "warning");
}

TEST_F(LaunchTest, ExistingConfigIsPreservedAndPathsUseItsDirectory)
{
    const auto text = valid_config() + "# keep this comment\n";
    write(config(), text);
    const auto settings = load_settings({});
    EXPECT_EQ(settings.catalogue, config().parent_path() / "catalogue.bin");
    EXPECT_TRUE(settings.log_file.empty());
    EXPECT_EQ(read(config()), text);
}

TEST_F(LaunchTest, RelativeExplicitConfigAndCliPathsUseWorkingDirectory)
{
    const auto path = root / "other/settings.toml";
    auto text = valid_config();
    text.replace(text.find("file = ''"), 9, "file = 'saved.log'");
    write(path, text);
    Overrides overrides;
    overrides.config = "other/settings.toml";
    EXPECT_EQ(load_settings(overrides).log_file, root / "other/saved.log");
    overrides.catalogue = "cli.bin";
    overrides.log_file = "cli.log";
    const auto settings = load_settings(overrides);
    EXPECT_EQ(settings.catalogue, root / "cli.bin");
    EXPECT_EQ(settings.log_file, root / "cli.log");
    EXPECT_EQ(read(path), text);
}

TEST_F(LaunchTest, DefaultLookingAndEmptyExplicitValuesOverride)
{
    auto text = valid_config();
    text.replace(text.find("4096"), 4, "9000");
    text.replace(text.find("warning"), 7, "debug");
    text.replace(text.find("file = ''"), 9, "file = 'saved.log'");
    write(config(), text);
    Overrides overrides;
    overrides.quantity = "4096";
    overrides.log_level = "warning";
    overrides.log_file = "";
    const auto settings = load_settings(overrides);
    EXPECT_EQ(settings.quantity, 4096);
    EXPECT_EQ(settings.log_level, "warning");
    EXPECT_TRUE(settings.log_file.empty());
}

TEST_F(LaunchTest, EmptyAndRelativeXdgValuesFallBackToHome)
{
    setenv("XDG_CONFIG_HOME", "relative", 1);
    setenv("XDG_DATA_HOME", "", 1);
    const auto settings = load_settings({});
    EXPECT_TRUE(std::filesystem::exists(
        root / "home/.config/mystarceiling/config.toml"));
    EXPECT_EQ(settings.catalogue,
              root / "home/.local/share/mystarceiling/catalogue.bin");
}

TEST_F(LaunchTest, AbsoluteXdgDoesNotRequireHome)
{
    unsetenv("HOME");
    EXPECT_NO_THROW((void)load_settings({}));
}

TEST_F(LaunchTest, MissingAbsoluteHomeForFallbackFails)
{
    unsetenv("XDG_CONFIG_HOME");
    setenv("HOME", "relative", 1);
    EXPECT_THROW((void)load_settings({}), std::runtime_error);
}

TEST_F(LaunchTest, TomlPathsDoNotExpandEnvironmentVariables)
{
    auto text = valid_config();
    text.replace(text.find("catalogue.bin"), 13, "$HOME/catalogue.bin");
    write(config(), text);
    EXPECT_EQ(load_settings({}).catalogue,
              config().parent_path() / "$HOME/catalogue.bin");
}

TEST_F(LaunchTest, InvalidConfigurationStaysUnchangedAndNamesSetting)
{
    const std::vector<std::pair<std::string, std::string>> cases{
        {"", "catalogue"},
        {valid_config().replace(valid_config().find("path = 'catalogue.bin'"),
                                22, "path = 7"),
         "catalogue.path"},
        {"[catalogue]\npath = ''\n[view]\nquantity = '64'\n[logging]\nfile = "
         "''\nlevel = 'warning'\n",
         "view.quantity"},
        {"[catalogue]\npath = ''\n[view]\nquantity = 64.0\n[logging]\n",
         "view.quantity"},
        {"[catalogue]\npath = ''\n[view]\nquantity = true\n[logging]\n",
         "view.quantity"},
        {"[catalogue]\npath = ''\n[view]\nquantity = -1\n[logging]\n",
         "view.quantity"},
        {valid_config() + "extra = true\n", "logging.extra"},
        {valid_config() + "[sources]\ngaia = ''\n", "sources"},
        {"catalogue = 3\n", "catalogue"},
        {"[catalogue]\npath = ''\n[view]\nquantity = 64\n", "logging"}};
    for (const auto& [text, setting] : cases)
    {
        write(config(), text);
        try
        {
            (void)load_settings({});
            FAIL() << text;
        }
        catch (const std::runtime_error& error)
        {
            EXPECT_NE(std::string{error.what()}.find(setting),
                      std::string::npos);
        }
        EXPECT_EQ(read(config()), text);
    }
}

TEST_F(LaunchTest, MalformedAndOverflowTomlRemainUnchanged)
{
    for (const auto& text :
         {"[view", "[view]\nquantity = 999999999999999999999999999\n"})
    {
        write(config(), text);
        try
        {
            (void)load_settings({});
            FAIL() << text;
        }
        catch (const std::runtime_error& error)
        {
            EXPECT_NE(std::string{error.what()}.find(config().string()),
                      std::string::npos);
            EXPECT_NE(std::string{error.what()}.find("line"),
                      std::string::npos);
        }
        EXPECT_EQ(read(config()), text);
    }
}

TEST_F(LaunchTest, OverridesCannotRepairMissingFieldsOrInvalidSavedLevel)
{
    Overrides overrides;
    overrides.quantity = "64";
    overrides.log_level = "warning";
    write(config(), "[catalogue]\npath = ''\n");
    EXPECT_THROW((void)load_settings(overrides), std::runtime_error);
    auto text = valid_config();
    text.replace(text.find("warning"), 7, "warn");
    write(config(), text);
    EXPECT_THROW((void)load_settings(overrides), std::runtime_error);
}

TEST_F(LaunchTest, RejectsInvalidExplicitQuantitiesAndLevels)
{
    write(config(), valid_config());
    for (const auto* value :
         {"", "-1", "+64", "64x", "1.5", "18446744073709551616"})
    {
        Overrides overrides;
        overrides.quantity = value;
        EXPECT_THROW((void)load_settings(overrides), std::runtime_error)
            << value;
    }
    Overrides overrides;
    overrides.log_level = "";
    EXPECT_THROW((void)load_settings(overrides), std::runtime_error);
    overrides = {};
    overrides.config = "";
    EXPECT_THROW((void)load_settings(overrides), std::runtime_error);
}

TEST_F(LaunchTest, EveryRequiredFieldHasStrictPresenceAndType)
{
    const std::vector<std::pair<std::string, std::string>> cases{
        {"path = 'catalogue.bin'", "catalogue.path"},
        {"quantity = 4096", "view.quantity"},
        {"file = ''", "logging.file"},
        {"level = 'warning'", "logging.level"}};
    for (const auto& [field, setting] : cases)
    {
        for (const auto* replacement : {"", "false"})
        {
            auto text = valid_config();
            const auto invalid = std::string{replacement}.empty()
                ? ""
                : field.substr(0, field.find('=') + 1) + replacement;
            text.replace(text.find(field), field.size(), invalid);
            write(config(), text);
            try
            {
                (void)load_settings({});
                FAIL() << setting;
            }
            catch (const std::runtime_error& error)
            {
                EXPECT_NE(std::string{error.what()}.find(setting),
                          std::string::npos);
            }
            EXPECT_EQ(read(config()), text);
        }
    }
}

TEST_F(LaunchTest, MissingExplicitConfigIsCreatedWithoutChangingDefaultLocation)
{
    Overrides overrides;
    overrides.config = "explicit/sub/config.toml";
    const auto settings = load_settings(overrides);
    EXPECT_TRUE(std::filesystem::exists(root / *overrides.config));
    EXPECT_FALSE(std::filesystem::exists(config()));
    EXPECT_EQ(settings.quantity, 4096);
    EXPECT_EQ(settings.catalogue, root / "data/mystarceiling/catalogue.bin");
}
