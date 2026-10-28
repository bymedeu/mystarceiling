#include <gtest/gtest.h>

#include <mystarceiling/catalogue/hipparcos_csv.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include "cli/m1_scene.hh"

#include <filesystem>
#include <mystarceiling/catalogue/gaia_csv.hh>
#include <mystarceiling/core/projection/zenith_perspective.hh>
#include <stdexcept>
#include <string>
#include <string_view>
#include <zlib.h>

using msc::catalogue::load_gaia_csv_gzip;
using msc::core::projection::project_zenith_perspective;

namespace
{
    constexpr std::string_view header = "source_id,ref_epoch,ra,dec,pmra,pmdec,"
                                        "parallax,phot_g_mean_mag,bp_rp\n";
    constexpr std::string_view known_visible_row =
        "160886283751041408,2016.0,74.24843702624405,33.16601472318802,"
        "3.8169754472223425,-17.786274545345467,7.249065034494049,"
        "2.1975422,1.4451852\n";

    class TemporaryGzip
    {
    public:
        explicit TemporaryGzip(std::string_view content) :
            filename_{std::filesystem::temp_directory_path() /
                      (std::string{"mystarceiling-catalogue-to-screen-"} +
                       ::testing::UnitTest::GetInstance()
                           ->current_test_info()
                           ->name() +
                       ".csv.gz")}
        {
            gzFile file = gzopen(filename_.c_str(), "wb");
            if (file == nullptr)
                throw std::runtime_error{"Could not create gzip fixture"};
            const int written = gzwrite(file, content.data(),
                                        static_cast<unsigned>(content.size()));
            const int close_result = gzclose(file);
            if (written != static_cast<int>(content.size()) ||
                close_result != Z_OK)
                throw std::runtime_error{"Could not write gzip fixture"};
        }

        ~TemporaryGzip()
        {
            std::filesystem::remove(filename_);
        }

        [[nodiscard]] const std::filesystem::path& filename() const
        {
            return filename_;
        }

    private:
        std::filesystem::path filename_;
    };
} // namespace

TEST(CatalogueToScreenTest, ProjectsKnownGaiaSourceForFixedM1Scene)
{
    const TemporaryGzip fixture{std::string{header} +
                                std::string{known_visible_row}};
    const auto loaded = load_gaia_csv_gzip(fixture.filename(), 1);
    ASSERT_TRUE(loaded.has_value()) << loaded.error().detail;
    ASSERT_EQ(loaded->stars.size(), 1);
    const auto scene = msc::cli::make_m1_scene(loaded->stars);
    ASSERT_EQ(scene.stars.size(), 1);
    const auto projected =
        project_zenith_perspective(scene.stars.front().enu, 1280.0 / 720.0);

    ASSERT_TRUE(projected.has_value());
    EXPECT_NEAR(projected->x, -0.23376923971854943, 1e-12);
    EXPECT_NEAR(projected->y, -0.12855289148878088, 1e-12);
}

TEST(CatalogueToScreenTest,
     PropagatesKnownCapellaRowBeforeFixedToulouseProjection)
{
    const TemporaryGzip fixture{
        "hip_id,ref_epoch,ra,dec,pmra,pmdec,parallax,hp_mag,b_v,v_i,"
        "estimated_g,g_estimation_method,resolution_reason\n"
        "24608,1991.25,79.17206465892026,45.999029047535174,75.25,-426.89,"
        "76.20,0.2385,0.795,0.830,-0.10273946410000001,edr3_hp_bv,"
        "no_published_best_neighbour\n"};
    const auto loaded =
        msc::catalogue::load_hipparcos_csv_gzip(fixture.filename());
    ASSERT_TRUE(loaded);
    const auto scene = msc::cli::make_m1_scene(loaded->stars);
    ASSERT_EQ(scene.stars.size(), 1);
    const auto projected =
        project_zenith_perspective(scene.stars.front().enu, 1280.0 / 720.0);
    ASSERT_TRUE(projected);
    // Independent spherical destination calculation: bearing atan2(mu_alpha*,
    // mu_delta), angular step atan(hypot(mu)*24.75), then fixed Toulouse hour
    // angle and perspective. Retain the historical 2e-8 projected-coordinate
    // reference budget (which included float output rounding); omitting motion
    // changes the result by much more than this tolerance.
    EXPECT_NEAR(projected->x, -0.1532045692891902, 2e-8);
    EXPECT_NEAR(projected->y, 0.08015878094575217, 2e-8);
    EXPECT_DOUBLE_EQ(
        loaded->stars.front().star().reference_epoch().julian_year(), 2016.0);
    EXPECT_NE(loaded->stars.front().star().colour().red,
              loaded->stars.front().star().colour().blue);
}
