#pragma once

#include <filesystem>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <string_view>

namespace msc::cli
{
    spdlog::level::level_enum logging_level(std::string_view name);

    class ScopedLogging
    {
    public:
        ScopedLogging(const std::filesystem::path& file,
                      std::string_view level);
        ~ScopedLogging();
        ScopedLogging(const ScopedLogging&) = delete;
        ScopedLogging& operator=(const ScopedLogging&) = delete;

    private:
        std::shared_ptr<spdlog::logger> previous_;
    };
} // namespace msc::cli
