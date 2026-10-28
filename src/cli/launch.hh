#pragma once

#include <functional>
#include <iosfwd>

#include "config.hh"
#include "m1_scene.hh"

namespace msc::cli
{
    [[nodiscard]] M1Scene prepare_view(const Settings& settings);
    int run_cli(int argc, const char* const* argv, std::ostream& output,
                std::ostream& errors,
                const std::function<void(const M1Scene&)>& show);
} // namespace msc::cli
