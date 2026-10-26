#pragma once

#include <cstddef>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/render/appearance_trial.hh>
#include <mystarceiling/render/opengl_renderer.hh>
#include <optional>
#include <span>
#include <vector>

namespace msc::cli
{
    struct M1Scene
    {
        std::vector<render::ScreenPoint> points;
        std::size_t propagation_failures{};
        std::size_t magnitude_excluded{};
        std::vector<render::TrialStar> trial_stars;
        std::optional<render::TrialSettings> trial;
    };

    // Temporary display policy, independent of astronomical visibility.
    [[nodiscard]] float m1_display_intensity(double magnitude) noexcept;
    [[nodiscard]] M1Scene
    make_m1_scene(std::span<const catalogue::CatalogueStar> stars);
    [[nodiscard]] M1Scene
    make_appearance_scene(std::span<const catalogue::CatalogueStar> stars,
                          const render::TrialSettings& settings);
} // namespace msc::cli
