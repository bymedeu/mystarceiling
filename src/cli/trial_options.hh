#pragma once

#include <mystarceiling/render/appearance_trial.hh>

namespace CLI
{
    class App;
}

namespace msc::cli
{
    struct TrialOptions
    {
        bool enabled{};
        render::TrialSettings settings;
    };

    void add_trial_options(CLI::App& view, TrialOptions& trial);
} // namespace msc::cli
