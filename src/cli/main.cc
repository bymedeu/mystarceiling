#include <iostream>
#include <mystarceiling/render/appearance_trial.hh>
#include <mystarceiling/render/opengl_renderer.hh>
#include <span>
#include <stdexcept>

#include "launch.hh"
#include "m1_scene.hh"

namespace
{
    void show_scene(const msc::cli::M1Scene& scene)
    {
        const auto result = scene.trial
            ? msc::render::show_appearance_trial(std::span{scene.trial_stars},
                                                 *scene.trial)
            : msc::render::show_star_window(std::span{scene.points});
        if (!result)
            throw std::runtime_error("Rendering failed: " + result.error());
    }
} // namespace

int main(int argc, char** argv)
{
    return msc::cli::run_cli(argc, argv, std::cout, std::cerr, show_scene);
}
