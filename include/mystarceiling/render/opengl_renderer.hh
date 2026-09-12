#pragma once

#include <expected>
#include <span>
#include <string>

namespace msc::render
{
    struct ScreenPoint
    {
        float x;
        float y;
        float intensity;
    };

    [[nodiscard]] std::expected<void, std::string>
    show_star_window(std::span<const ScreenPoint> points);
} // namespace msc::render
