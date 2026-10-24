#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>

namespace msc::cli
{
    struct Settings
    {
        std::filesystem::path catalogue;
        std::size_t quantity{4096};
        std::filesystem::path log_file;
        std::string log_level{"warning"};
    };

    struct Overrides
    {
        std::optional<std::string> config;
        std::optional<std::string> catalogue;
        std::optional<std::string> quantity;
        std::optional<std::string> log_file;
        std::optional<std::string> log_level;
    };

    [[nodiscard]] Settings load_settings(const Overrides& overrides);
} // namespace msc::cli
