#pragma once

#include "appearance_trial_private.hh"

#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <mystarceiling/render/appearance_trial.hh>

namespace msc::render::test
{
    using Pixels = std::vector<std::uint8_t>;
    using SourcePixel = std::array<float, 3>;
    using DisplayPixel = std::array<std::uint8_t, 4>;

    // Declaration order keeps Window alive until every GL resource is released,
    // including on fatal assertions. No production window/event loop runs.
    struct OpenGlContext
    {
        trial::Window window;
        std::optional<trial::Resources> resources;
        trial::State state{};

        [[nodiscard]] std::expected<void, std::string> open();
        void initialize_scene(std::span<const TrialStar> stars);
        void draw();
        void toggle_picture();
        [[nodiscard]] Pixels pixels() const;
        [[nodiscard]] SourcePixel source_pixel(int x, int y) const;
    };

    [[nodiscard]] DisplayPixel display_pixel(int x, int y);
} // namespace msc::render::test
