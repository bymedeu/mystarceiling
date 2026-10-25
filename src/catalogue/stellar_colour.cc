#include <mystarceiling/catalogue/stellar_colour.hh>

#include <algorithm>
#include <cmath>
#include <expected>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>

namespace msc::catalogue
{
    namespace
    {
        // IEC sRGB decoding; the temperature fit returns display-encoded
        // values.
        double linear_channel(double encoded) noexcept
        {
            const auto value = std::clamp(encoded / 255.0, 0.0, 1.0);
            return value <= 0.04045 ? value / 12.92
                                    : std::pow((value + 0.055) / 1.055, 2.4);
        }

        double gaia_temperature(double bp_rp) noexcept
        {
            // Jordi cubic, with Gaia Sky's explicit red appearance extension.
            // Limits bound this surrogate, not the retained source measurement.
            const auto colour = std::clamp(bp_rp, -0.5, 15.0);
            const auto polynomial = [](double x)
            {
                return std::pow(10.0,
                                3.999 + x * (-0.654 + x * (0.709 - 0.316 * x)));
            };
            if (colour <= 1.5)
                return polynomial(colour);
            const auto boundary = polynomial(1.5);
            return boundary + (colour - 1.5) / 13.5 * (3000.0 - boundary);
        }
    } // namespace

    std::expected<LinearRgb8, ColourError>
    colour_from_temperature_kelvin(double temperature) noexcept
    {
        if (!std::isfinite(temperature) || temperature <= 0)
            return std::unexpected{ColourError::InvalidChannel};
        // Original Helland photographic fit, 1000--40000 K. The exact method
        // and scientific limitations are recorded in
        // docs/decisions/stellar-colour.md.
        const auto t = std::clamp(temperature, 1000.0, 40000.0) / 100.0;
        const auto red =
            t <= 66 ? 255.0 : 329.698727446 * std::pow(t - 60.0, -0.1332047592);
        const auto green = t <= 66
            ? 99.4708025861 * std::log(t) - 161.1195681661
            : 288.1221695283 * std::pow(t - 60.0, -0.0755148492);
        const auto blue = t >= 66 ? 255.0
            : t <= 19             ? 0.0
                      : 138.5177312231 * std::log(t - 10.0) - 305.0447927307;
        return LinearRgb8::try_from_linear(
            {linear_channel(red), linear_channel(green), linear_channel(blue)});
    }

    std::expected<LinearRgb8, ColourError>
    colour_from_gaia_bp_rp(double bp_rp) noexcept
    {
        if (!std::isfinite(bp_rp))
            return std::unexpected{ColourError::InvalidChannel};
        return colour_from_temperature_kelvin(gaia_temperature(bp_rp));
    }

    std::expected<LinearRgb8, ColourError>
    colour_from_hipparcos_b_v(double b_v) noexcept
    {
        if (!std::isfinite(b_v))
            return std::unexpected{ColourError::InvalidChannel};
        // Ballesteros (2012), with appearance-only bounds avoiding
        // singularities.
        const auto colour = std::clamp(b_v, -0.4, 2.0);
        const auto temperature = 4600.0 *
            (1.0 / (0.92 * colour + 1.7) + 1.0 / (0.92 * colour + 0.62));
        return colour_from_temperature_kelvin(temperature);
    }
} // namespace msc::catalogue
