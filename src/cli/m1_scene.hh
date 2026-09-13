#pragma once

#include <cstddef>
#include <mystarceiling/catalogue/star.hh>
#include <mystarceiling/render/opengl_renderer.hh>
#include <span>
#include <vector>

namespace msc::cli
{
    struct M1Scene
    {
        std::vector<render::ScreenPoint> points;
        std::size_t propagation_failures{};
        std::size_t magnitude_excluded{};
    };

    // Temporary display policy, independent of astronomical visibility.
    [[nodiscard]] float m1_display_intensity(double magnitude) noexcept;
    [[nodiscard]] M1Scene make_m1_scene(std::span<const catalogue::Star> stars);
} // namespace msc::cli
