#pragma once

#include <expected>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <span>
#include <string>

namespace msc::render
{
    struct ScreenPoint
    {
        float x{};
        float y{};
        float intensity{}; // Linear-light display intensity.
        core::astronomy::LinearRgb8 colour{255, 255, 255};
    };

    [[nodiscard]] std::expected<void, std::string>
    show_star_window(std::span<const ScreenPoint> points);
} // namespace msc::render
