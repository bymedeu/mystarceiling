#include "trial_options.hh"

#include <CLI/CLI.hpp>

namespace msc::cli
{
    namespace
    {
        void add_profile_options(CLI::App& app, render::TrialSettings& value,
                                 CLI::Option* enable)
        {
            app.add_option("--brightness", value.brightness,
                           "Linear-light display scale (0.01..1000)")
                ->needs(enable);
            app.add_option("--limiting-magnitude", value.limiting_magnitude,
                           "Render loaded stars up to this G-like magnitude")
                ->needs(enable);
            app.add_option("--core-width", value.core_width,
                           "Core Gaussian sigma in framebuffer pixels")
                ->needs(enable);
            app.add_option("--halo-width", value.halo_width,
                           "Soft halo sigma in framebuffer pixels")
                ->needs(enable);
            app.add_option(
                   "--halo-fraction", value.halo_fraction,
                   "Fraction of stellar light assigned to the soft halo")
                ->needs(enable);
            app.add_option("--ray-fraction", value.ray_fraction,
                           "Fraction of stellar light assigned to rays")
                ->needs(enable);
            app.add_option("--ray-length", value.ray_length,
                           "Longest ray sigma in framebuffer pixels")
                ->needs(enable);
            app.add_option("--ray-width", value.ray_width,
                           "Ray transverse sigma in framebuffer pixels")
                ->needs(enable);
            app.add_option("--ray-count", value.ray_count,
                           "Unequal directional rays (3..24)")
                ->needs(enable);
            app.add_option("--ray-rotation", value.ray_rotation,
                           "Common ray-pattern rotation in radians")
                ->needs(enable);
            app.add_option("--profile-support", value.support_sigma,
                           "Finite profile extent in sigma (2..6)")
                ->needs(enable);
        }

        void add_animation_options(CLI::App& app, render::TrialSettings& value,
                                   CLI::Option* enable)
        {
            app.add_option("--twinkle-strength", value.twinkle.strength,
                           "Maximum horizon brightness modulation (0..<1)")
                ->needs(enable);
            app.add_option("--colour-variation", value.twinkle.colour_variation,
                           "Maximum horizon chromatic modulation (0..<1)")
                ->needs(enable);
            app.add_option("--twinkle-zenith-fraction",
                           value.twinkle.zenith_fraction,
                           "Zenith modulation relative to horizon (0..1)")
                ->needs(enable);
            app.add_option("--twinkle-slow-rate", value.twinkle.slow_rate,
                           "Slow noise target updates per animation second")
                ->needs(enable);
            app.add_option("--twinkle-fast-rate", value.twinkle.fast_rate,
                           "Fast noise target updates per animation second")
                ->needs(enable);
            app.add_option("--twinkle-slow-weight", value.twinkle.slow_weight,
                           "Slow component weight (0..1)")
                ->needs(enable);
            app.add_option("--animation-speed", value.animation_speed,
                           "Animation clock speed multiplier (0.01..10)")
                ->needs(enable);
            app.add_option("--seed", value.seed,
                           "Deterministic animation and ray-pattern seed")
                ->needs(enable);
            app.add_flag("--picture-mode", value.picture_mode,
                         "Start with the current picture frozen")
                ->needs(enable);
        }
    } // namespace

    void add_trial_options(CLI::App& view, TrialOptions& trial)
    {
        auto* group = view.add_option_group(
            "Appearance trial",
            "Provisional native preview; session-only settings");
        auto* enable =
            group->add_flag("--appearance-trial", trial.enabled,
                            "Preview animated stellar light and rays");
        add_profile_options(*group, trial.settings, enable);
        add_animation_options(*group, trial.settings, enable);
        group
            ->add_option("--width", trial.settings.width,
                         "Initial window width (64..4096)")
            ->needs(enable);
        group
            ->add_option("--height", trial.settings.height,
                         "Initial window height (64..2160)")
            ->needs(enable);
    }
} // namespace msc::cli
