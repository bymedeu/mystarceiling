#include <gtest/gtest.h>

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <random>
#include <string>
#include <system_error>
#include <vector>

using msc::catalogue::BinaryCatalogueErrorCode;
using msc::catalogue::CatalogueStar;
using msc::catalogue::read_binary_catalogue;
using msc::catalogue::write_binary_catalogue;
using msc::core::astronomy::ApparentMagnitude;
using msc::core::astronomy::LinearRgb8;
using msc::core::astronomy::Parallax;
using msc::core::astronomy::Star;
using msc::core::coordinates::Declination;
using msc::core::coordinates::IcrsDirection;
using msc::core::coordinates::ProperMotion;
using msc::core::coordinates::ProperMotionComponents;
using msc::core::coordinates::RightAscension;
using msc::core::maths::Angle;
using msc::core::time::ReferenceEpoch;

namespace
{
    // Independent little-endian version-3 fixture: two provenance values,
    // full-width ID, signed measurements, and distinct linear RGB bytes.
    constexpr std::array<unsigned char, 168> expected_bytes{
        0x4d, 0x53, 0x43, 0x53, 0x54, 0x41, 0x52, 0x00, 0x03, 0x00, 0x20, 0x00,
        0x03, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x9f, 0x40,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x40, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x40, 0x34, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x40,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xc0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xd0, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x40,
        0xff, 0x80, 0x00, 0x00, 0x5d, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x9f, 0x40, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x25, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x34, 0xc0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x40, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x12, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0xbf,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x40, 0x40, 0x80, 0xff, 0x01,
    };
    void make_selected_stars(std::vector<CatalogueStar>& stars)
    {
        const auto epoch = ReferenceEpoch::try_from_julian_year(2016.0);
        const auto ra = RightAscension::try_from(Angle::from_degrees(10.5));
        const auto dec = Declination::try_from(Angle::from_degrees(-20.25));
        const auto pmra = ProperMotion::try_from_milliarcseconds_per_year(3.5);
        const auto pmdec =
            ProperMotion::try_from_milliarcseconds_per_year(-4.5);
        const auto parallax = Parallax::try_from_milliarcseconds(-0.25);
        const auto magnitude = ApparentMagnitude::try_from_gaia_g(5.75);
        ASSERT_TRUE(epoch && ra && dec && pmra && pmdec && parallax &&
                    magnitude);
        stars = {
            CatalogueStar{std::numeric_limits<std::uint64_t>::max(),
                          msc::catalogue::CatalogueSource::GaiaDr3,
                          Star{*epoch, IcrsDirection{*ra, *dec},
                               ProperMotionComponents{
                                   .right_ascension_cos_declination = *pmra,
                                   .declination = *pmdec},
                               *parallax, *magnitude, LinearRgb8{255, 128, 0}}},
            CatalogueStar{32349, msc::catalogue::CatalogueSource::Hipparcos2,
                          Star{*epoch, IcrsDirection{*ra, *dec},
                               ProperMotionComponents{*pmra, *pmdec}, *parallax,
                               *magnitude, LinearRgb8{64, 128, 255}}}};
    }

    void expect_selected_values(const std::vector<CatalogueStar>& loaded)
    {
        EXPECT_EQ(loaded[1].source_id(), 32349);
        EXPECT_EQ(loaded[1].source_catalogue(),
                  msc::catalogue::CatalogueSource::Hipparcos2);
        EXPECT_EQ(loaded[1].star().colour(), (LinearRgb8{64, 128, 255}));
        const auto& star = loaded.front().star();
        EXPECT_EQ(loaded.front().source_catalogue(),
                  msc::catalogue::CatalogueSource::GaiaDr3);
        EXPECT_EQ(loaded.front().source_id(),
                  std::numeric_limits<std::uint64_t>::max());
        EXPECT_DOUBLE_EQ(star.reference_epoch().julian_year(), 2016.0);
        EXPECT_DOUBLE_EQ(
            star.direction().get_right_ascension().get_angle().to_degrees(),
            10.5);
        EXPECT_DOUBLE_EQ(
            star.direction().get_declination().get_angle().to_degrees(),
            -20.25);
        EXPECT_DOUBLE_EQ(
            star.proper_motion()
                .right_ascension_cos_declination.milliarcseconds_per_year(),
            3.5);
        EXPECT_DOUBLE_EQ(
            star.proper_motion().declination.milliarcseconds_per_year(), -4.5);
        EXPECT_DOUBLE_EQ(star.parallax().milliarcseconds(), -0.25);
        EXPECT_DOUBLE_EQ(star.magnitude().gaia_g(), 5.75);
        EXPECT_EQ(star.colour(), (LinearRgb8{255, 128, 0}));
    }
} // namespace

TEST(BinaryCatalogueTest, PreservesAllSelectedValues)
{
    std::vector<CatalogueStar> stars;
    ASSERT_NO_FATAL_FAILURE(make_selected_stars(stars));
    const auto filename = std::filesystem::temp_directory_path() /
        "mystarceiling-catalogue-test.bin";

    ASSERT_TRUE(write_binary_catalogue(filename, stars).has_value());
    {
        std::ifstream bytes{filename, std::ios::binary};
        for (const auto expected : expected_bytes)
            EXPECT_EQ(bytes.get(), expected);
        EXPECT_EQ(bytes.get(), std::char_traits<char>::eof());
    }
    const auto loaded = read_binary_catalogue(filename);
    std::filesystem::remove(filename);

    ASSERT_TRUE(loaded.has_value()) << loaded.error().detail;
    ASSERT_EQ(loaded->size(), 2);
    ASSERT_NO_FATAL_FAILURE(expect_selected_values(*loaded));
}

TEST(BinaryCatalogueTest, ReadsIndependentVersion3Fixture)
{
    const auto filename = std::filesystem::temp_directory_path() /
        "mystarceiling-catalogue-fixture-test.bin";
    {
        std::ofstream file{filename, std::ios::binary};
        for (const auto byte : expected_bytes)
            file.put(static_cast<char>(byte));
        ASSERT_TRUE(file);
    }
    const auto loaded = read_binary_catalogue(filename);
    std::filesystem::remove(filename);
    ASSERT_TRUE(loaded) << loaded.error().detail;
    ASSERT_EQ(loaded->size(), 2);
    ASSERT_NO_FATAL_FAILURE(expect_selected_values(*loaded));
}

TEST(BinaryCatalogueTest, RejectsPreviousVersionsRatherThanGuessingSchema)
{
    const auto filename = std::filesystem::temp_directory_path() /
        "mystarceiling-old-catalogue-test.bin";
    for (const char version : {char{1}, char{2}})
    {
        ASSERT_TRUE(write_binary_catalogue(filename, {}));
        {
            std::fstream file{filename,
                              std::ios::in | std::ios::out | std::ios::binary};
            file.seekp(8);
            file.put(version);
            file.put(0);
        }
        const auto loaded = read_binary_catalogue(filename);
        ASSERT_FALSE(loaded);
        EXPECT_EQ(loaded.error().code,
                  BinaryCatalogueErrorCode::UnsupportedVersion);
    }
    std::filesystem::remove(filename);
}

namespace
{
    class BoundedBinaryCatalogueTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            std::random_device random;
            do
            {
                directory = std::filesystem::temp_directory_path() /
                    ("msc-reader-" + std::to_string(random()));
            }
            while (!std::filesystem::create_directory(directory));
            filename = directory / "catalogue.bin";
            std::vector<CatalogueStar> pair;
            ASSERT_NO_FATAL_FAILURE(make_selected_stars(pair));
            for (std::size_t index = 0; index < 66; ++index)
            {
                const auto& base = pair[index % 2];
                const auto& star = base.star();
                stars.emplace_back(1000 - index, base.source_catalogue(),
                                   Star{star.reference_epoch(),
                                        star.direction(), star.proper_motion(),
                                        star.parallax(),
                                        *ApparentMagnitude::try_from_gaia_g(
                                            static_cast<double>(66 - index)),
                                        star.colour()});
            }
            ASSERT_TRUE(write_binary_catalogue(filename, stars));
        }

        void TearDown() override
        {
            std::error_code cleanup_error;
            std::filesystem::remove_all(directory, cleanup_error);
            EXPECT_FALSE(cleanup_error) << cleanup_error.message();
        }

        void replace_byte(std::streamoff offset, char value)
        {
            std::fstream file{filename,
                              std::ios::binary | std::ios::in | std::ios::out};
            file.seekp(offset);
            file.put(value);
            ASSERT_TRUE(file);
        }

        void expect_failure(BinaryCatalogueErrorCode code)
        {
            const auto prefix = read_binary_catalogue(filename, 64);
            ASSERT_FALSE(prefix);
            EXPECT_EQ(prefix.error().code, code);
            const auto full = read_binary_catalogue(filename);
            ASSERT_FALSE(full);
            EXPECT_EQ(full.error().code, code);
        }

        std::filesystem::path directory;
        std::filesystem::path filename;
        std::vector<CatalogueStar> stars;
    };

    void expect_same_record(const CatalogueStar& actual,
                            const CatalogueStar& expected)
    {
        EXPECT_EQ(actual.source_id(), expected.source_id());
        EXPECT_EQ(actual.source_catalogue(), expected.source_catalogue());
        const auto& star = actual.star();
        const auto& original = expected.star();
        EXPECT_EQ(star.reference_epoch().julian_year(),
                  original.reference_epoch().julian_year());
        EXPECT_EQ(
            star.direction().get_right_ascension().get_angle().to_degrees(),
            original.direction()
                .get_right_ascension()
                .get_angle()
                .to_degrees());
        EXPECT_EQ(
            star.direction().get_declination().get_angle().to_degrees(),
            original.direction().get_declination().get_angle().to_degrees());
        EXPECT_EQ(
            star.proper_motion()
                .right_ascension_cos_declination.milliarcseconds_per_year(),
            original.proper_motion()
                .right_ascension_cos_declination.milliarcseconds_per_year());
        EXPECT_EQ(
            star.proper_motion().declination.milliarcseconds_per_year(),
            original.proper_motion().declination.milliarcseconds_per_year());
        EXPECT_EQ(star.parallax().milliarcseconds(),
                  original.parallax().milliarcseconds());
        EXPECT_EQ(star.magnitude().gaia_g(), original.magnitude().gaia_g());
        EXPECT_EQ(star.colour(), original.colour());
    }
} // namespace

TEST_F(BoundedBinaryCatalogueTest, PreservesExactPrefixAndFullCountInFileOrder)
{
    for (const std::size_t quantity : {64, 65, 66})
    {
        const auto loaded = read_binary_catalogue(filename, quantity);
        ASSERT_TRUE(loaded) << loaded.error().detail;
        ASSERT_EQ(loaded->size(), quantity);
        for (std::size_t index = 0; index < quantity; ++index)
            expect_same_record((*loaded)[index], stars[index]);
    }
}

TEST_F(BoundedBinaryCatalogueTest, RejectsInvalidQuantitiesWithCounts)
{
    for (const auto quantity :
         {std::size_t{0}, std::size_t{63}, std::size_t{67},
          std::numeric_limits<std::size_t>::max()})
    {
        const auto loaded = read_binary_catalogue(filename, quantity);
        ASSERT_FALSE(loaded);
        EXPECT_EQ(loaded.error().code,
                  BinaryCatalogueErrorCode::InvalidQuantity);
        EXPECT_NE(loaded.error().detail.find(std::to_string(quantity)),
                  std::string::npos);
        EXPECT_NE(loaded.error().detail.find("available 66"),
                  std::string::npos);
    }
}

TEST_F(BoundedBinaryCatalogueTest, RequiresMinimumEvenForSmallerCatalogues)
{
    stars.resize(63, stars.front());
    ASSERT_TRUE(write_binary_catalogue(filename, stars));
    for (const std::size_t quantity : {63, 64})
    {
        const auto loaded = read_binary_catalogue(filename, quantity);
        ASSERT_FALSE(loaded);
        EXPECT_EQ(loaded.error().code,
                  BinaryCatalogueErrorCode::InvalidQuantity);
    }
    const auto full = read_binary_catalogue(filename);
    ASSERT_TRUE(full);
    EXPECT_EQ(full->size(), 63);
}

TEST_F(BoundedBinaryCatalogueTest, AcceptsExactlyMinimumCatalogue)
{
    stars.resize(64, stars.front());
    ASSERT_TRUE(write_binary_catalogue(filename, stars));
    const auto loaded = read_binary_catalogue(filename, 64);
    ASSERT_TRUE(loaded);
    EXPECT_EQ(loaded->size(), 64);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsInvalidLastSelectedRecord)
{
    // The provenance byte of the last selected record is invalid.
    replace_byte(32 + 63 * 68 + 67, 2);
    expect_failure(BinaryCatalogueErrorCode::InvalidRecord);
}

TEST_F(BoundedBinaryCatalogueTest, IgnoresInvalidFirstUnusedRecordValues)
{
    // The provenance byte of the first unused record is invalid.
    replace_byte(32 + 64 * 68 + 67, 2);
    const auto prefix = read_binary_catalogue(filename, 64);
    ASSERT_TRUE(prefix);
    ASSERT_EQ(prefix->size(), 64);
    for (std::size_t index = 0; index < prefix->size(); ++index)
        expect_same_record((*prefix)[index], stars[index]);
    const auto full = read_binary_catalogue(filename);
    ASSERT_FALSE(full);
    EXPECT_EQ(full.error().code, BinaryCatalogueErrorCode::InvalidRecord);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsTruncatedUnusedTail)
{
    std::filesystem::resize_file(filename, 32 + 66 * 68 - 1);
    expect_failure(BinaryCatalogueErrorCode::InvalidFileSize);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsExtraTrailingByte)
{
    std::filesystem::resize_file(filename, 32 + 66 * 68 + 1);
    expect_failure(BinaryCatalogueErrorCode::InvalidFileSize);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsHeaderCountMismatch)
{
    replace_byte(24, 65);
    expect_failure(BinaryCatalogueErrorCode::InvalidFileSize);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsHeaderCountOverflow)
{
    for (std::streamoff offset = 24; offset < 32; ++offset)
        replace_byte(offset, static_cast<char>(0xff));
    expect_failure(BinaryCatalogueErrorCode::InvalidFileSize);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsTruncatedHeader)
{
    std::filesystem::resize_file(filename, 31);
    expect_failure(BinaryCatalogueErrorCode::ReadFailed);
}

TEST_F(BoundedBinaryCatalogueTest, ValidatesEveryHeaderFieldForPrefix)
{
    struct Mutation
    {
        std::streamoff offset;
        char original;
        BinaryCatalogueErrorCode code;
    };
    for (const auto& mutation :
         {Mutation{0, 'M', BinaryCatalogueErrorCode::InvalidMagic},
          Mutation{8, 3, BinaryCatalogueErrorCode::UnsupportedVersion},
          Mutation{10, 32, BinaryCatalogueErrorCode::UnsupportedVersion},
          Mutation{12, 3, BinaryCatalogueErrorCode::UnsupportedSchema},
          Mutation{16, 68, BinaryCatalogueErrorCode::InvalidRecordSize}})
    {
        replace_byte(mutation.offset, 0);
        expect_failure(mutation.code);
        replace_byte(mutation.offset, mutation.original);
    }
}

TEST_F(BoundedBinaryCatalogueTest, RejectsNonfiniteSelectedMeasurement)
{
    const auto bits =
        std::bit_cast<std::uint64_t>(std::numeric_limits<double>::quiet_NaN());
    for (std::size_t byte = 0; byte < sizeof(bits); ++byte)
        replace_byte(static_cast<std::streamoff>(32 + 63 * 68 + 8 + byte),
                     static_cast<char>((bits >> (byte * 8)) & 0xffU));
    expect_failure(BinaryCatalogueErrorCode::InvalidRecord);
}

TEST_F(BoundedBinaryCatalogueTest, IgnoresNonfiniteUnusedMeasurement)
{
    const auto bits =
        std::bit_cast<std::uint64_t>(std::numeric_limits<double>::quiet_NaN());
    for (std::size_t byte = 0; byte < sizeof(bits); ++byte)
        replace_byte(static_cast<std::streamoff>(32 + 64 * 68 + 8 + byte),
                     static_cast<char>((bits >> (byte * 8)) & 0xffU));
    const auto prefix = read_binary_catalogue(filename, 64);
    ASSERT_TRUE(prefix);
    EXPECT_EQ(prefix->size(), 64);
    const auto full = read_binary_catalogue(filename);
    ASSERT_FALSE(full);
    EXPECT_EQ(full.error().code, BinaryCatalogueErrorCode::InvalidRecord);
}

TEST_F(BoundedBinaryCatalogueTest, PreservesFineMagnitudeAndColourDifferences)
{
    stars.resize(2, stars.front());
    const auto& base = stars.front().star();
    const auto magnitude =
        ApparentMagnitude::try_from_gaia_g(base.magnitude().gaia_g() + 0.00001);
    ASSERT_TRUE(magnitude);
    stars[1] = CatalogueStar{stars[1].source_id(), stars[1].source_catalogue(),
                             Star{base.reference_epoch(), base.direction(),
                                  base.proper_motion(), base.parallax(),
                                  *magnitude, LinearRgb8{254, 129, 1}}};
    ASSERT_TRUE(write_binary_catalogue(filename, stars));
    const auto loaded = read_binary_catalogue(filename);
    ASSERT_TRUE(loaded) << loaded.error().detail;
    ASSERT_EQ(loaded->size(), 2);
    EXPECT_NE((*loaded)[0].star().magnitude().gaia_g(),
              (*loaded)[1].star().magnitude().gaia_g());
    for (std::size_t index = 0; index < loaded->size(); ++index)
        expect_same_record((*loaded)[index], stars[index]);
    EXPECT_EQ((*loaded)[0].star().colour(), (LinearRgb8{255, 128, 0}));
    EXPECT_EQ((*loaded)[1].star().colour(), (LinearRgb8{254, 129, 1}));
}

TEST_F(BoundedBinaryCatalogueTest, RejectsNonJ2016SelectedEpoch)
{
    const auto bits = std::bit_cast<std::uint64_t>(1991.25);
    for (std::size_t byte = 0; byte < sizeof(bits); ++byte)
        replace_byte(static_cast<std::streamoff>(32 + 63 * 68 + 8 + byte),
                     static_cast<char>((bits >> (byte * 8)) & 0xffU));
    expect_failure(BinaryCatalogueErrorCode::InvalidRecord);
}

TEST_F(BoundedBinaryCatalogueTest, RejectsNonJ2016BeforeOpeningOutput)
{
    const auto& base = stars.front().star();
    const auto epoch = ReferenceEpoch::try_from_julian_year(1991.25);
    ASSERT_TRUE(epoch);
    const std::vector<CatalogueStar> invalid{CatalogueStar{
        stars.front().source_id(), stars.front().source_catalogue(),
        Star{*epoch, base.direction(), base.proper_motion(), base.parallax(),
             base.magnitude(), base.colour()}}};
    const auto missing = directory / "must-not-be-created.bin";
    for (const auto& output : {filename, missing})
    {
        const auto written = write_binary_catalogue(output, invalid);
        ASSERT_FALSE(written);
        EXPECT_EQ(written.error().code,
                  BinaryCatalogueErrorCode::InvalidRecord);
    }
    EXPECT_FALSE(std::filesystem::exists(missing));
    const auto loaded = read_binary_catalogue(filename);
    ASSERT_TRUE(loaded) << loaded.error().detail;
    ASSERT_EQ(loaded->size(), stars.size());
    for (std::size_t index = 0; index < loaded->size(); ++index)
        expect_same_record((*loaded)[index], stars[index]);
}
