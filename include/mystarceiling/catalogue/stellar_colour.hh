#pragma once

#include <expected>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>

namespace msc::catalogue
{
    using core::astronomy::ColourError;
    using core::astronomy::LinearRgb8;

    // Approximate observed colour surrogates; limits affect appearance only.
    [[nodiscard]] std::expected<LinearRgb8, ColourError>
    colour_from_gaia_bp_rp(double bp_rp) noexcept;
    [[nodiscard]] std::expected<LinearRgb8, ColourError>
    colour_from_hipparcos_b_v(double b_v) noexcept;
    [[nodiscard]] std::expected<LinearRgb8, ColourError>
    colour_from_temperature_kelvin(double temperature) noexcept;
} // namespace msc::catalogue
