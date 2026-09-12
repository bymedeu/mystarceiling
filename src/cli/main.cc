#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <span>
#include <vector>

#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <mystarceiling/catalogue/gaia_csv.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/projection/zenith_perspective.hh>
#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>
#include <mystarceiling/render/opengl_renderer.hh>

using msc::catalogue::load_gaia_csv_gzip;
using msc::catalogue::read_binary_catalogue;
using msc::catalogue::write_binary_catalogue;
using msc::core::coordinates::icrs_to_enu;
using msc::core::coordinates::Latitude;
using msc::core::coordinates::Longitude;
using msc::core::coordinates::ObserverLocation;
using msc::core::maths::Angle;
using msc::core::projection::project_zenith_perspective;
using msc::core::time::earth_rotation_angle;
using msc::core::time::JulianDate;
using msc::render::ScreenPoint;
using msc::render::show_star_window;

namespace {
[[nodiscard]] float display_intensity(double magnitude,
                                      double brightest_magnitude) {
  const double relative_flux =
      std::pow(10.0, -0.4 * (magnitude - brightest_magnitude));
  const double display_value = std::sqrt(relative_flux);
  return static_cast<float>(std::clamp(display_value, 0.15, 1.0));
}
} // namespace

int main(int argc, char **argv) {
  constexpr std::size_t star_count = 4096;
  constexpr double aspect_ratio = 1280.0 / 720.0;

  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <gaia.csv.gz> <runtime-catalogue.bin>\n";
    return EXIT_FAILURE;
  }

  const std::filesystem::path source_path{argv[1]};
  const std::filesystem::path runtime_path{argv[2]};
  const auto parsed = load_gaia_csv_gzip(source_path, star_count);
  if (!parsed) {
    std::cerr << "Catalogue load failed: " << parsed.error().detail << '\n';
    return EXIT_FAILURE;
  }

  const auto write_result = write_binary_catalogue(runtime_path, parsed->stars);
  if (!write_result) {
    std::cerr << "Binary catalogue write failed: "
              << write_result.error().detail << '\n';
    return EXIT_FAILURE;
  }
  const auto stars = read_binary_catalogue(runtime_path);
  if (!stars) {
    std::cerr << "Binary catalogue read failed: " << stars.error().detail
              << '\n';
    return EXIT_FAILURE;
  }

  const auto latitude = Latitude::try_from(Angle::from_degrees(43.6045));
  const auto longitude = Longitude::try_from(Angle::from_degrees(1.4440));
  if (!latitude || !longitude) {
    std::cerr << "Internal Toulouse observer coordinates are invalid\n";
    return EXIT_FAILURE;
  }
  const ObserverLocation observer{*latitude, *longitude};

  // 2016-01-01 00:00:00 UTC. M1 treats UTC as UT1, an approximation whose
  // sub-second rotation error is immaterial for this first visual slice.
  const JulianDate ut1{2457388.5, 0.0};
  const auto earth_rotation = earth_rotation_angle(ut1);

  const auto brightest = std::min_element(
      stars->begin(), stars->end(), [](const auto &left, const auto &right) {
        return left.magnitude().gaia_g() < right.magnitude().gaia_g();
      });
  const double brightest_magnitude =
      brightest == stars->end() ? 0.0 : brightest->magnitude().gaia_g();

  std::vector<ScreenPoint> points;
  points.reserve(stars->size());
  for (const auto &star : *stars) {
    const auto enu = icrs_to_enu(star.direction(), observer, earth_rotation);
    const auto projected = project_zenith_perspective(enu, aspect_ratio);
    if (projected)
      points.push_back(ScreenPoint{
          static_cast<float>(projected->x), static_cast<float>(projected->y),
          display_intensity(star.magnitude().gaia_g(), brightest_magnitude)});
  }

  std::cout << "Accepted " << parsed->statistics.accepted << " of "
            << parsed->statistics.rows_seen << " Gaia rows; projected "
            << points.size() << " stars into the 90-degree view.\n";
  if (!stars->empty())
    std::cout << "First Gaia source_id: " << stars->front().source_id() << '\n';

  const auto render_result = show_star_window(std::span{points});
  if (!render_result) {
    std::cerr << "Rendering failed: " << render_result.error() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
