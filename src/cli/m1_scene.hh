#pragma once

#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/render/appearance_trial.hh>
#include <span>
#include <vector>

namespace msc::cli
{
    struct M1Scene
    {
        std::vector<render::TrialStar> stars;
        render::TrialSettings settings;
    };

    [[nodiscard]] M1Scene
    make_m1_scene(std::span<const catalogue::CatalogueStar> stars);
} // namespace msc::cli
