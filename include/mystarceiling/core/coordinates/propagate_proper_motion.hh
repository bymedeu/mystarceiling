#pragma once

#include <expected>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/time/reference_epoch.hh>

namespace msc::core::coordinates
{
    enum class ProperMotionPropagationError
    {
        NonFiniteEpochDifference,
        NonFiniteDisplacement,
        InvalidDirection
    };

    // Normalize direction + elapsed years * tangent motion.
    // Epochs must use the same time scale; one Julian year is 365.25 days.
    // No distance, radial velocity, parallax, or observer effects are modeled.
    // At an input pole, RA defines the tangent basis. An exact output pole has
    // RA zero, except that zero time or zero motion preserves the input.
    [[nodiscard]] std::expected<IcrsDirection, ProperMotionPropagationError>
    propagate_proper_motion(IcrsDirection direction,
                            ProperMotionComponents motion,
                            time::ReferenceEpoch reference_epoch,
                            time::ReferenceEpoch target_epoch) noexcept;
} // namespace msc::core::coordinates
