#include <mystarceiling/core/projection/zenith_perspective.hh>

#include <cmath>
#include <mystarceiling/core/maths/vector3.hh>
#include <optional>

namespace msc::core::projection {
std::optional<ProjectedDirection>
project_zenith_perspective(maths::Vector3 direction,
                           double aspect_ratio) noexcept {
  if (!std::isfinite(aspect_ratio) || aspect_ratio <= 0.0 ||
      direction.z() <= 0.0)
    return std::nullopt;

  // The vertical half-angle is 45 degrees, so tan(fov_y / 2) is one.
  const double x = direction.x() / (direction.z() * aspect_ratio);
  const double y = direction.y() / direction.z();
  if (!std::isfinite(x) || !std::isfinite(y) || std::abs(x) > 1.0 ||
      std::abs(y) > 1.0)
    return std::nullopt;

  return ProjectedDirection{x, y};
}
} // namespace msc::core::projection
