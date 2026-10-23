#pragma once

#include <mystarceiling/core/maths/vector3.hh>
#include <optional>

namespace msc::core::projection
{
    struct ProjectedDirection
    {
        double x;
        double y;
    };

    [[nodiscard]] std::optional<ProjectedDirection>
    project_zenith_perspective(const maths::Vector3& enu_direction,
                               double aspect_ratio) noexcept;
} // namespace msc::core::projection
