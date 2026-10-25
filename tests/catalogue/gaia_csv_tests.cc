#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/catalogue/gaia_csv.hh>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <stdexcept>
#include <string>
#include <string_view>
#include <zlib.h>

using msc::catalogue::GaiaRowError;
using msc::catalogue::load_gaia_csv_gzip;

namespace
{
    constexpr std::string_view header = "source_id,ref_epoch,ra,dec,pmra,pmdec,"
                                        "parallax,phot_g_mean_mag,bp_rp\n";
    constexpr std::string_view known_row =
        "418551920284673408,2016.0,10.12724197930297,56.53718879378639,"
        "49.12564571845064,-31.595374311196693,14.090976249252234,"
        "1.9425238,1.1434835\n";

    class TemporaryGzip
    {
    public:
        explicit TemporaryGzip(std::string_view content) :
            filename_{std::filesystem::temp_directory_path() /
                      (std::string{"mystarceiling-gaia-test-"} +
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
        const std::filesystem::path& filename() const
        {
            return filename_;
        }

    private:
        std::filesystem::path filename_;
    };
} // namespace

TEST(GaiaCatalogueTest, InterpretsKnownGaiaRow)
{
    const TemporaryGzip fixture{std::string{header} + std::string{known_row}};
    const auto result = load_gaia_csv_gzip(fixture.filename(), 1);

    ASSERT_TRUE(result.has_value()) << result.error().detail;
    ASSERT_EQ(result->stars.size(), 1);
    const auto& star = result->stars.front();
    EXPECT_EQ(star.source_id(), 418551920284673408ULL);
    EXPECT_EQ(star.source_catalogue(),
              msc::catalogue::CatalogueSource::GaiaDr3);
    EXPECT_DOUBLE_EQ(star.star().reference_epoch().julian_year(), 2016.0);
    EXPECT_DOUBLE_EQ(
        star.star().direction().get_right_ascension().get_angle().to_degrees(),
        10.12724197930297);
    EXPECT_DOUBLE_EQ(
        star.star().direction().get_declination().get_angle().to_degrees(),
        56.53718879378639);
    EXPECT_DOUBLE_EQ(
        star.star()
            .proper_motion()
            .right_ascension_cos_declination.milliarcseconds_per_year(),
        49.12564571845064);
    EXPECT_DOUBLE_EQ(
        star.star().proper_motion().declination.milliarcseconds_per_year(),
        -31.595374311196693);
    EXPECT_DOUBLE_EQ(star.star().parallax().milliarcseconds(),
                     14.090976249252234);
    EXPECT_DOUBLE_EQ(star.star().magnitude().gaia_g(), 1.9425238);
    EXPECT_EQ(star.star().colour(),
              (msc::core::astronomy::LinearRgb8{255, 201, 162}));
}

TEST(GaiaCatalogueTest, RejectsBadRowsAndContinuesToAcceptedLimit)
{
    const std::string content = std::string{header} +
        "1,2016.0,,0,1,2,3,4,5\n" + "2,2016.0,400,0,1,2,3,4,5\n" +
        std::string{known_row};
    const TemporaryGzip fixture{content};
    const auto result = load_gaia_csv_gzip(fixture.filename(), 1);

    ASSERT_TRUE(result.has_value()) << result.error().detail;
    EXPECT_EQ(result->statistics.rows_seen, 3);
    EXPECT_EQ(result->statistics.accepted, 1);
    EXPECT_EQ(result->statistics.rejected_total(), 2);
    EXPECT_EQ(
        result->statistics
            .rejected[static_cast<std::size_t>(GaiaRowError::MissingValue)],
        1);
    EXPECT_EQ(result->statistics.rejected[static_cast<std::size_t>(
                  GaiaRowError::InvalidRightAscension)],
              1);
}

TEST(GaiaCatalogueTest, PreservesNegativeParallax)
{
    const TemporaryGzip fixture{std::string{header} +
                                "3,2016.0,10,-20,1,2,-0.5,4,0.2\n"};
    const auto result = load_gaia_csv_gzip(fixture.filename(), 1);

    ASSERT_TRUE(result.has_value()) << result.error().detail;
    ASSERT_EQ(result->stars.size(), 1);
    EXPECT_DOUBLE_EQ(result->stars.front().star().parallax().milliarcseconds(),
                     -0.5);
}

TEST(GaiaCatalogueTest, PreparesNonstandardEpochAndKeepsMagnitudePrecision)
{
    const TemporaryGzip fixture{std::string{header} +
                                "3,1991.25,0,0,3600000,0,1,4.00001,1.0\n" +
                                "4,2016,0,0,0,0,1,4.00002,1.0\n"};
    const auto result = load_gaia_csv_gzip(fixture.filename(), 2);
    ASSERT_TRUE(result);
    ASSERT_EQ(result->stars.size(), 2);
    const auto& first = result->stars.front().star();
    EXPECT_DOUBLE_EQ(first.reference_epoch().julian_year(), 2016.0);
    // Tangent displacement of 24.75 degrees; the model normalises that vector.
    EXPECT_NEAR(
        first.direction().get_right_ascension().get_angle().to_degrees(),
        23.36284714437749, 1e-12);
    EXPECT_DOUBLE_EQ(first.magnitude().gaia_g(), 4.00001);
    EXPECT_DOUBLE_EQ(result->stars[1].star().magnitude().gaia_g(), 4.00002);
    EXPECT_NE(first.magnitude().gaia_g(),
              result->stars[1].star().magnitude().gaia_g());
}

TEST(GaiaCatalogueTest, RejectsFailedPreparationAndContinuesToAcceptedLimit)
{
    const TemporaryGzip fixture{
        std::string{header} +
        "1,-1.7976931348623157e308,0,0,1.7976931348623157e308,0,1,4,1\n" +
        std::string{known_row}};
    const auto result = load_gaia_csv_gzip(fixture.filename(), 1);
    ASSERT_TRUE(result);
    ASSERT_EQ(result->stars.size(), 1);
    EXPECT_EQ(result->statistics.rejected[static_cast<std::size_t>(
                  GaiaRowError::InvalidPropagation)],
              1);
}
