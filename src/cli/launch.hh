#pragma once

#include <functional>
#include <iosfwd>
#include <optional>

#include "config.hh"
#include "m1_scene.hh"

namespace msc::cli
{
    [[nodiscard]] M1Scene prepare_view(
        const Settings& settings,
        const std::optional<render::TrialSettings>& trial = std::nullopt);
    int run_cli(int argc, const char* const* argv, std::ostream& output,
                std::ostream& errors,
                const std::function<void(const M1Scene&)>& show);
} // namespace msc::cli
