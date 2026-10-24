#include "cli/logging.hh"
#include "launch_test_support.hh"

#include <array>
#include <cstddef>
#include <filesystem>
#include <gtest/gtest.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

using msc::cli::ScopedLogging;
using msc::cli::test::LaunchTest;

TEST_F(LaunchTest, FileLoggingTruncatesOnceAndHonorsDefaultWarningThreshold)
{
    const auto path = root / "run.log";
    write(path, "old contents");
    {
        const ScopedLogging logging{path, "warning"};
        spdlog::info("hidden message");
        spdlog::warn("first warning");
        spdlog::error("second error");
        EXPECT_EQ(spdlog::get_level(), spdlog::level::warn);
    }
    const auto text = read(path);
    EXPECT_EQ(text.find("old contents"), std::string::npos);
    EXPECT_EQ(text.find("hidden message"), std::string::npos);
    EXPECT_NE(text.find("first warning"), std::string::npos);
    EXPECT_NE(text.find("second error"), std::string::npos);
    {
        const ScopedLogging next_run{path, "off"};
        spdlog::critical("hidden");
    }
    EXPECT_TRUE(read(path).empty());
}

TEST_F(LaunchTest, AllSupportedThresholdsControlOrdinarySpdlogCalls)
{
    constexpr std::array names{"trace", "debug",    "info", "warning",
                               "error", "critical", "off"};
    for (std::size_t threshold = 0; threshold < names.size(); ++threshold)
    {
        const auto path = root / "new.log";
        {
            const ScopedLogging logging{path, names[threshold]};
            for (std::size_t level = 0; level < names.size() - 1; ++level)
                spdlog::log(static_cast<spdlog::level::level_enum>(level),
                            "message{}", level);
        }
        const auto text = read(path);
        for (std::size_t level = 0; level < names.size() - 1; ++level)
            EXPECT_EQ(text.find("message" + std::to_string(level)) !=
                          std::string::npos,
                      level >= threshold);
    }
}

TEST_F(LaunchTest, EmptyDestinationUsesNormalStderrSinkAndRestoresLogger)
{
    const auto previous = spdlog::default_logger();
    const auto level = spdlog::get_level();
    {
        const ScopedLogging logging{{}, "debug"};
        EXPECT_NE(spdlog::default_logger(), previous);
        const auto& sinks = spdlog::default_logger()->sinks();
        ASSERT_EQ(sinks.size(), 1);
        EXPECT_NE(
            dynamic_cast<spdlog::sinks::stderr_sink_mt*>(sinks.front().get()),
            nullptr);
        EXPECT_EQ(spdlog::get_level(), spdlog::level::debug);
        EXPECT_EQ(previous->level(), level);
    }
    EXPECT_EQ(spdlog::default_logger(), previous);
    EXPECT_EQ(spdlog::get(previous->name()), previous);
    EXPECT_EQ(spdlog::get_level(), level);
}

TEST_F(LaunchTest, OpenFailureAndInvalidLevelLeaveLoggerAndFileUnchanged)
{
    const auto previous = spdlog::default_logger();
    const auto level = spdlog::get_level();
    EXPECT_THROW((ScopedLogging{root, "warning"}), spdlog::spdlog_ex);
    const auto path = root / "keep.log";
    write(path, "keep");
    EXPECT_THROW((ScopedLogging{path, "warn"}), std::runtime_error);
    EXPECT_EQ(read(path), "keep");
    EXPECT_EQ(spdlog::default_logger(), previous);
    EXPECT_EQ(spdlog::get(previous->name()), previous);
    EXPECT_EQ(spdlog::get_level(), level);
}
