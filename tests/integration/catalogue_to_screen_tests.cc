#include <gtest/gtest.h>

#include <filesystem>
#include <mystarceiling/catalogue/gaia_csv.hh>
#include <mystarceiling/core/coordinates/icrs_to_enu.hh>
#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
#include <mystarceiling/core/coordinates/observer_location.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/projection/zenith_perspective.hh>
#include <mystarceiling/core/time/earth_rotation.hh>
#include <mystarceiling/core/time/julian_date.hh>
#include <stdexcept>
#include <string>
#include <string_view>
#include <zlib.h>

using msc::catalogue::load_gaia_csv_gzip;
using msc::core::coordinates::icrs_to_enu;
using msc::core::coordinates::Latitude;
using msc::core::coordinates::Longitude;
using msc::core::coordinates::ObserverLocation;
using msc::core::maths::Angle;
using msc::core::projection::project_zenith_perspective;
using msc::core::time::earth_rotation_angle;
using msc::core::time::JulianDate;

namespace {
constexpr std::string_view header = "source_id,ref_epoch,ra,dec,pmra,pmdec,"
                                    "parallax,phot_g_mean_mag,bp_rp\n";
constexpr std::string_view known_visible_row =
    "160886283751041408,2016.0,74.24843702624405,33.16601472318802,"
    "3.8169754472223425,-17.786274545345467,7.249065034494049,"
    "2.1975422,1.4451852\n";

class TemporaryGzip {
public:
  explicit TemporaryGzip(std::string_view content)
      : filename_{std::filesystem::temp_directory_path() /
                  "mystarceiling-catalogue-to-screen.csv.gz"} {
    gzFile file = gzopen(filename_.c_str(), "wb");
    if (file == nullptr)
      throw std::runtime_error{"Could not create gzip fixture"};
    const int written =
        gzwrite(file, content.data(), static_cast<unsigned>(content.size()));
    const int close_result = gzclose(file);
    if (written != static_cast<int>(content.size()) || close_result != Z_OK)
      throw std::runtime_error{"Could not write gzip fixture"};
  }

  ~TemporaryGzip() { std::filesystem::remove(filename_); }

  [[nodiscard]] const std::filesystem::path &filename() const {
    return filename_;
  }

private:
  std::filesystem::path filename_;
};
} // namespace

TEST(CatalogueToScreenTest, ProjectsKnownGaiaSourceForFixedM1Scene) {
  const TemporaryGzip fixture{std::string{header} +
                              std::string{known_visible_row}};
  const auto loaded = load_gaia_csv_gzip(fixture.filename(), 1);
  ASSERT_TRUE(loaded.has_value()) << loaded.error().detail;
  ASSERT_EQ(loaded->stars.size(), 1);
  const auto &star = loaded->stars.front();

  const auto latitude = Latitude::try_from(Angle::from_degrees(43.6045));
  const auto longitude = Longitude::try_from(Angle::from_degrees(1.4440));
  ASSERT_TRUE(latitude && longitude);

  const ObserverLocation observer{*latitude, *longitude};
  const auto rotation = earth_rotation_angle(JulianDate{2457388.5, 0.0});
  const auto enu = icrs_to_enu(star.direction(), observer, rotation);
  const auto projected = project_zenith_perspective(enu, 1280.0 / 720.0);

  ASSERT_TRUE(projected.has_value());
  EXPECT_NEAR(projected->x, -0.23376923971854943, 1e-12);
  EXPECT_NEAR(projected->y, -0.12855289148878088, 1e-12);
}
