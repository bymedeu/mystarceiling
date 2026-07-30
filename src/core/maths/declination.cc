#include <cmath>
#include <mystarceiling/core/maths/declination.hh>
#include <numbers>

namespace mystarceiling::core::maths
{
    std::expected<Declination, DeclinationError>
    Declination::try_from(const Angle& angle)
    {
        const auto rad = angle.to_radian();
        constexpr double halfpi = std::numbers::pi / 2;
        if (!std::isfinite(rad))
            return std::unexpected{DeclinationError::NonFinite};

        if (rad < -halfpi || rad > halfpi)
            return std::unexpected{DeclinationError::OutOfRange};

        return Declination{angle};
    }

    [[nodiscard]] Angle Declination::get_angle() const noexcept
    {
        return angle_;
    }

    Declination::Declination(const Angle& angle) : angle_{angle}
    {}

} // namespace mystarceiling::core::maths
