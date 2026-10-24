#include "logging.hh"

#include <array>
#include <filesystem>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace msc::cli
{
    spdlog::level::level_enum logging_level(std::string_view name)
    {
        constexpr std::array levels{
            std::pair{"trace", spdlog::level::trace},
            std::pair{"debug", spdlog::level::debug},
            std::pair{"info", spdlog::level::info},
            std::pair{"warning", spdlog::level::warn},
            std::pair{"error", spdlog::level::err},
            std::pair{"critical", spdlog::level::critical},
            std::pair{"off", spdlog::level::off}};
        for (const auto& [label, level] : levels)
            if (name == label)
                return level;
        throw std::runtime_error("Invalid logging.level: " + std::string{name});
    }

    ScopedLogging::ScopedLogging(const std::filesystem::path& file,
                                 std::string_view level) :
        previous_{spdlog::default_logger()}
    {
        const auto threshold = logging_level(level);
        spdlog::sink_ptr sink;
        if (file.empty())
            sink = std::make_shared<spdlog::sinks::stderr_sink_mt>();
        else
            sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                file.string(), true);
        auto logger = std::make_shared<spdlog::logger>(previous_->name(),
                                                       std::move(sink));
        logger->set_level(threshold);
        logger->flush_on(spdlog::level::trace);
        spdlog::set_default_logger(std::move(logger));
    }

    ScopedLogging::~ScopedLogging()
    {
        spdlog::set_default_logger(std::move(previous_));
    }
} // namespace msc::cli
