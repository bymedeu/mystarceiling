#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <mystarceiling/catalogue/hipparcos_csv.hh>
#include <stdexcept>
#include <string>
#include <string_view>
#include <zlib.h>

using msc::catalogue::HipparcosRowError;
using msc::catalogue::load_hipparcos_csv_gzip;

namespace
{
    constexpr std::string_view header =
        "hip_id,ref_epoch,ra,dec,pmra,pmdec,parallax,hp_mag,b_v,v_i,"
        "estimated_g,g_estimation_method,resolution_reason\n";
    constexpr std::string_view known_row =
        "32349,1991.25,101.2885410520664,-16.713143062644765,-546.01,"
        "-1223.07,379.21,-1.0876,0.009,-0.020,-1.1151784320607998,"
        "edr3_hp_bv,no_published_best_neighbour\n";

    class TemporaryGzip
    {
    public:
        explicit TemporaryGzip(std::string_view content) :
            filename_{std::filesystem::temp_directory_path() /
                      (std::string{"mystarceiling-hipparcos-test-"} +
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


TEST(HipparcosCatalogueTest, InterpretsSiriusWithoutInventingBpRp)
{
    const TemporaryGzip fixture{std::string{header} + std::string{known_row}};
    const auto result = load_hipparcos_csv_gzip(fixture.filename());
    ASSERT_TRUE(result) << result.error().detail;
    ASSERT_EQ(result->stars.size(), 1);
    const auto& star = result->stars.front();
    EXPECT_EQ(star.source_id(), 32349);
    EXPECT_EQ(star.source_catalogue(),
              msc::catalogue::CatalogueSource::Hipparcos2);
    EXPECT_FALSE(star.colour());
    EXPECT_DOUBLE_EQ(star.reference_epoch().julian_year(), 1991.25);
    EXPECT_DOUBLE_EQ(
        star.direction().get_right_ascension().get_angle().to_degrees(),
        101.2885410520664);
    EXPECT_DOUBLE_EQ(
        star.direction().get_declination().get_angle().to_degrees(),
        -16.713143062644765);
    EXPECT_DOUBLE_EQ(
        star.proper_motion()
            .right_ascension_cos_declination.milliarcseconds_per_year(),
        -546.01);
    EXPECT_DOUBLE_EQ(
        star.proper_motion().declination.milliarcseconds_per_year(), -1223.07);
    EXPECT_DOUBLE_EQ(star.parallax().milliarcseconds(), 379.21);
    EXPECT_DOUBLE_EQ(star.magnitude().gaia_g(), -1.1151784320607998);
}

TEST(HipparcosCatalogueTest, RejectsMalformedRowsAndContinues)
{
    const TemporaryGzip fixture{
        std::string{header} + "1,1991.25,0,0,0,0,1,2,3,4,5,method\n" +
        "2,1991.25,0,0,0,0,1,2,3,4,,method,reason\n" +
        "0,1991.25,0,0,0,0,1,2,3,4,5,method,reason\n" +
        "3,1991.25,0,0,0,0,1,2,3,4,nan,method,reason\n" +
        "4,1991.25,360,0,0,0,1,2,3,4,5,method,reason\n" +
        "5,1991.25,0,91,0,0,1,2,3,4,5,method,reason\n" +
        std::string{known_row}};
    const auto result = load_hipparcos_csv_gzip(fixture.filename());
    ASSERT_TRUE(result);
    EXPECT_EQ(result->statistics.accepted, 1);
    EXPECT_EQ(result->statistics.rows_seen, 7);
    for (const auto count : result->statistics.rejected)
        EXPECT_EQ(count, 1);
}

TEST(HipparcosCatalogueTest, RejectsGaiaHeader)
{
    const TemporaryGzip fixture{"source_id,ref_epoch,ra,dec,pmra,pmdec,"
                                "parallax,phot_g_mean_mag,bp_rp\n"};
    const auto result = load_hipparcos_csv_gzip(fixture.filename());
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code,
              msc::catalogue::HipparcosFileErrorCode::InvalidHeader);
}
