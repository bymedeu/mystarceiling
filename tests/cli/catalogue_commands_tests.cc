#include "cli/config.hh"
#include "cli/launch.hh"
#include "cli/m1_scene.hh"
#include "launch_test_support.hh"

#include <algorithm>
#include <array>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <ios>
#include <iterator>
#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <zlib.h>

namespace
{
    using msc::catalogue::CatalogueSource;
    using msc::catalogue::read_binary_catalogue;
    using msc::catalogue::read_binary_catalogue_info;

    class CatalogueCommandTest : public msc::cli::test::LaunchTest
    {
    protected:
        void SetUp() override
        {
            LaunchTest::SetUp();
            auto text = valid_config();
            text.replace(text.find("capacity = 4096"), 15, "capacity = 64");
            text.replace(text.find("quantity = 4096"), 15, "quantity = 64");
            write(root / "settings.toml", text);
        }

        int run(std::vector<std::string> arguments)
        {
            arguments.insert(arguments.begin(),
                             {"msc", "--config", "settings.toml"});
            std::vector<const char*> argv;
            std::ranges::transform(arguments, std::back_inserter(argv),
                                   [](const auto& argument)
                                   { return argument.c_str(); });
            output.str("");
            errors.str("");
            return msc::cli::run_cli(argv.size(), argv.data(), output, errors,
                                     [&](const msc::cli::M1Scene&)
                                     { shown = true; });
        }

        void gzip(const std::filesystem::path& path, std::string_view content,
                  const char* mode = "wb")
        {
            gzFile file = gzopen(path.c_str(), mode);
            ASSERT_NE(file, nullptr);
            const auto result = gzwrite(file, content.data(),
                                        static_cast<unsigned>(content.size()));
            const auto closed = gzclose(file);
            ASSERT_EQ(result, static_cast<int>(content.size()));
            ASSERT_EQ(closed, Z_OK);
        }

        void sources(std::size_t gaia_count, std::size_t hipparcos_count,
                     bool rejected = false)
        {
            std::string gaia = "source_id,ref_epoch,ra,dec,pmra,pmdec,parallax,"
                               "phot_g_mean_mag,bp_rp\n";
            std::string hipparcos =
                "hip_id,ref_epoch,ra,dec,pmra,pmdec,parallax,hp_mag,b_v,v_i,"
                "estimated_g,g_estimation_method,resolution_reason\n";
            if (rejected)
            {
                gaia += "bad,row\n";
                hipparcos += "bad,row\n";
            }
            for (std::size_t i = 1; i <= gaia_count; ++i)
                gaia += std::to_string(i) + ",2016,10.5,45,0,0,1," +
                    std::to_string(i) + ",0.5\n";
            for (std::size_t i = 1; i <= hipparcos_count; ++i)
                hipparcos += std::to_string(i) +
                    ",1991.25,10.5,45,0,0,1,1,0,0," + std::to_string(1000 - i) +
                    ",method,reason\n";
            gzip(root / "gaia.csv.gz", gaia);
            gzip(root / "hipparcos.csv.gz", hipparcos);
        }

        void no_temporary_files()
        {
            for (const auto& entry :
                 std::filesystem::recursive_directory_iterator(root))
                EXPECT_FALSE(entry.path().filename().string().starts_with(
                    ".msc-catalogue-"));
        }

        std::ostringstream output;
        std::ostringstream errors;
        bool shown = false;
    };
} // namespace

TEST_F(CatalogueCommandTest, DefaultsBuildExactHipparcosFirstSelectionThenView)
{
    sources(100, 2);
    ASSERT_EQ(run({"catalogue", "build"}), 0) << errors.str();
    EXPECT_FALSE(shown);
    const auto records = read_binary_catalogue(root / "catalogue.bin");
    ASSERT_TRUE(records);
    ASSERT_EQ(records->size(), 64);
    EXPECT_EQ((*records)[61].source_catalogue(), CatalogueSource::GaiaDr3);
    EXPECT_EQ((*records)[61].source_id(), 62);
    EXPECT_EQ((*records)[62].source_catalogue(), CatalogueSource::Hipparcos2);
    EXPECT_EQ((*records)[62].source_id(), 2);
    EXPECT_EQ((*records)[63].source_id(), 1);
    std::filesystem::remove(root / "gaia.csv.gz");
    std::filesystem::remove(root / "hipparcos.csv.gz");
    EXPECT_EQ(run({"view"}), 0) << errors.str();
    EXPECT_TRUE(shown);
    no_temporary_files();
}

TEST_F(CatalogueCommandTest, HipparcosAtOrAboveCapacityDoesNotOpenGaia)
{
    for (const auto count : {64, 80})
    {
        sources(0, count);
        std::filesystem::remove(root / "gaia.csv.gz");
        ASSERT_EQ(run({"catalogue", "build", "--force"}), 0) << errors.str();
        const auto records = read_binary_catalogue(root / "catalogue.bin");
        ASSERT_TRUE(records);
        ASSERT_EQ(records->size(), 64);
        EXPECT_EQ(records->front().source_id(), count);
        EXPECT_EQ(records->back().source_id(), count - 63);
        for (const auto& record : *records)
            EXPECT_EQ(record.source_catalogue(), CatalogueSource::Hipparcos2);
    }
}

TEST_F(CatalogueCommandTest, GaiaStopsAfterRemainingAcceptedRows)
{
    sources(100, 2);
    gzip(root / "gaia.csv.gz", "bad,row\n", "ab");
    ASSERT_EQ(run({"catalogue", "build", "--log-file", "tail.log"}), 0)
        << errors.str();
    EXPECT_TRUE(read(root / "tail.log").empty());
    const auto records = read_binary_catalogue(root / "catalogue.bin");
    ASSERT_TRUE(records);
    EXPECT_EQ(
        std::count_if(
            records->begin(), records->end(), [](const auto& record)
            { return record.source_catalogue() == CatalogueSource::GaiaDr3; }),
        62);
}

TEST_F(CatalogueCommandTest, RejectsShortfallAndPreservesExistingBinary)
{
    sources(10, 2);
    write(root / "catalogue.bin", "keep old catalogue");
    EXPECT_NE(run({"catalogue", "build", "--force"}), 0);
    EXPECT_NE(errors.str().find("Requested 64 records; available 12"),
              std::string::npos);
    EXPECT_EQ(errors.str().find("Retry:"), std::string::npos);
    EXPECT_EQ(read(root / "catalogue.bin"), "keep old catalogue");
    no_temporary_files();
}

TEST_F(CatalogueCommandTest, ShortfallShowsSameCommandWithKnownValidCapacity)
{
    sources(62, 2);
    EXPECT_NE(
        run({"catalogue", "build", "--capacity=80", "--log-level", "off"}), 0);
    EXPECT_NE(errors.str().find("Requested 80 records; available 64"),
              std::string::npos);
    EXPECT_NE(errors.str().find("'msc' '--config' 'settings.toml' 'catalogue' "
                                "'build' '--log-level' 'off' --capacity 64"),
              std::string::npos);
    EXPECT_FALSE(std::filesystem::exists(root / "catalogue.bin"));
}

TEST_F(CatalogueCommandTest, RejectedRowsAreSkippedAndReportedTogether)
{
    sources(62, 2, true);
    ASSERT_EQ(run({"catalogue", "build", "--log-file", "rows.log"}), 0)
        << errors.str();
    EXPECT_NE(
        read(root / "rows.log").find("Hipparcos: skipped 1 unusable rows"),
        std::string::npos);
    EXPECT_NE(read(root / "rows.log").find("Gaia: skipped 1 unusable rows"),
              std::string::npos);
    EXPECT_EQ(read_binary_catalogue_info(root / "catalogue.bin")->records, 64);
}

TEST_F(CatalogueCommandTest, OverwriteNeedsForceAndForceReplacesWholeCatalogue)
{
    sources(62, 2);
    write(root / "catalogue.bin", "keep");
    EXPECT_NE(run({"catalogue", "build"}), 0);
    EXPECT_NE(errors.str().find("--force"), std::string::npos);
    EXPECT_EQ(read(root / "catalogue.bin"), "keep");
    EXPECT_EQ(run({"catalogue", "build", "--force"}), 0) << errors.str();
    EXPECT_TRUE(read_binary_catalogue(root / "catalogue.bin"));
    no_temporary_files();
}

TEST_F(CatalogueCommandTest, RejectsSourceDestinationAliasesEvenWithForce)
{
    sources(62, 2);
    const auto original = read(root / "gaia.csv.gz");
    std::filesystem::create_symlink(root / "gaia.csv.gz", root / "alias.gz");
    std::filesystem::create_hard_link(root / "gaia.csv.gz", root / "hard.gz");
    for (const auto* target : {"gaia.csv.gz", "hipparcos.csv.gz", "alias.gz",
                               "hard.gz", "settings.toml"})
    {
        EXPECT_NE(run({"catalogue", "build", "--catalogue", target, "--force"}),
                  0);
        EXPECT_NE(errors.str().find("must not overwrite"), std::string::npos);
    }
    EXPECT_EQ(read(root / "gaia.csv.gz"), original);
    no_temporary_files();
}

TEST_F(CatalogueCommandTest, LogCannotTruncateInputsOutputOrConfiguration)
{
    sources(62, 2);
    const auto gaia = read(root / "gaia.csv.gz");
    for (const auto* path :
         {"gaia.csv.gz", "hipparcos.csv.gz", "catalogue.bin", "settings.toml"})
    {
        EXPECT_NE(run({"catalogue", "build", "--log-file", path}), 0);
        EXPECT_NE(errors.str().find("Log file must not overwrite"),
                  std::string::npos);
    }
    EXPECT_EQ(read(root / "gaia.csv.gz"), gaia);
}

TEST_F(CatalogueCommandTest, InvalidCapacityAndSourceFileFailuresDoNotPublish)
{
    sources(62, 2);
    for (const auto* capacity :
         {"0", "63", "-1", "64x", "1.5", "18446744073709551616"})
        EXPECT_NE(run({"catalogue", "build", "--capacity", capacity}), 0);
    EXPECT_NE(run({"catalogue", "build", "--hipparcos", "missing.gz"}), 0);
    EXPECT_NE(errors.str().find("Hipparcos load failed"), std::string::npos);
    gzip(root / "gaia.csv.gz", "invalid header\n");
    EXPECT_NE(run({"catalogue", "build"}), 0);
    EXPECT_NE(errors.str().find("Gaia load failed"), std::string::npos);
    EXPECT_FALSE(std::filesystem::exists(root / "catalogue.bin"));
    no_temporary_files();
}

TEST_F(CatalogueCommandTest,
       OverridesSourcesCapacityAndDestinationWithoutSaving)
{
    sources(100, 2);
    std::filesystem::rename(root / "gaia.csv.gz", root / "other-gaia.gz");
    std::filesystem::rename(root / "hipparcos.csv.gz", root / "other-hip.gz");
    const auto saved = read(root / "settings.toml");
    EXPECT_EQ(run({"catalogue", "build", "--gaia", "other-gaia.gz",
                   "--hipparcos", "other-hip.gz", "--capacity", "80",
                   "--catalogue", "nested/other.bin"}),
              0)
        << errors.str();
    EXPECT_EQ(read_binary_catalogue_info(root / "nested/other.bin")->records,
              80);
    EXPECT_EQ(read(root / "settings.toml"), saved);
    EXPECT_FALSE(std::filesystem::exists(root / "catalogue.bin"));
}

TEST_F(CatalogueCommandTest, InfoUsesOnlyHeaderAndLengthAndPrintsDecimalGb)
{
    sources(62, 2);
    ASSERT_EQ(run({"catalogue", "build"}), 0) << errors.str();
    // Deliberately invalidate the first record; info must not decode it.
    std::fstream binary{root / "catalogue.bin",
                        std::ios::in | std::ios::out | std::ios::binary};
    binary.seekp(32);
    binary.write("\0\0\0\0\0\0\0\0", 8);
    binary.close();
    std::filesystem::remove(root / "gaia.csv.gz");
    std::filesystem::remove(root / "hipparcos.csv.gz");
    ASSERT_EQ(run({"catalogue", "info"}), 0) << errors.str();
    EXPECT_EQ(output.str(),
              "Catalogue: " + (root / "catalogue.bin").string() +
                  "\nRecords: 64\nFile size: 4384 bytes (0.00000438 GB)\n");
    EXPECT_FALSE(read_binary_catalogue(root / "catalogue.bin"));
    EXPECT_FALSE(shown);
}

TEST_F(CatalogueCommandTest, InfoRejectsIncompatibleHeaderAndWrongLength)
{
    sources(62, 2);
    ASSERT_EQ(run({"catalogue", "build"}), 0) << errors.str();
    const auto saved = read(root / "catalogue.bin");
    for (const auto offset : {0, 8, 10, 12, 16, 24})
    {
        auto invalid = saved;
        invalid[offset] ^= 0x7f;
        write(root / "catalogue.bin", invalid);
        EXPECT_NE(run({"catalogue", "info"}), 0) << offset;
        EXPECT_TRUE(output.str().empty());
    }
    write(root / "catalogue.bin", saved + "x");
    EXPECT_NE(run({"catalogue", "info"}), 0);
    write(root / "catalogue.bin", saved.substr(0, 30));
    EXPECT_NE(run({"catalogue", "info"}), 0);
}

TEST_F(CatalogueCommandTest, InfoUsesThreeDecimalsForLargerSparseBinary)
{
    sources(62, 2);
    ASSERT_EQ(run({"catalogue", "build"}), 0) << errors.str();
    std::fstream binary{root / "catalogue.bin",
                        std::ios::in | std::ios::out | std::ios::binary};
    binary.seekp(24);
    std::uint64_t count = 20000000;
    for (int i = 0; i < 8; ++i)
    {
        binary.put(static_cast<char>(count & 0xff));
        count >>= 8;
    }
    binary.close();
    std::filesystem::resize_file(root / "catalogue.bin", 1360000032);
    ASSERT_EQ(run({"catalogue", "info"}), 0) << errors.str();
    EXPECT_NE(output.str().find("1360000032 bytes (1.360 GB)"),
              std::string::npos);
}

TEST_F(CatalogueCommandTest,
       WriteFailureRemovesTemporaryAndPreservesOldCatalogue)
{
    sources(62, 2);
    write(root / "catalogue.bin", "keep");
    const auto child = ::fork();
    ASSERT_GE(child, 0);
    if (child == 0)
    {
        std::signal(SIGXFSZ, SIG_IGN);
        const rlimit limit{256, 256};
        if (::setrlimit(RLIMIT_FSIZE, &limit) != 0)
            ::_exit(2);
        const auto result = run({"catalogue", "build", "--force"});
        ::_exit(result != 0 &&
                        errors.str().find("write failed") != std::string::npos
                    ? 0
                    : 3);
    }
    int status{};
    ASSERT_EQ(::waitpid(child, &status, 0), child);
    ASSERT_TRUE(WIFEXITED(status));
    EXPECT_EQ(WEXITSTATUS(status), 0);
    EXPECT_EQ(read(root / "catalogue.bin"), "keep");
    no_temporary_files();
}

TEST_F(CatalogueCommandTest,
       PathAliasCreatesMissingDestinationAndWorksForInfoAndView)
{
    sources(62, 2);
    const auto saved = read(root / "settings.toml");
    const auto destination = root / "new/nested/catalogue.bin";
    ASSERT_FALSE(std::filesystem::exists(destination.parent_path()));
    ASSERT_EQ(run({"catalogue", "build", "--path", "new/nested/catalogue.bin"}),
              0)
        << errors.str();
    ASSERT_TRUE(std::filesystem::is_regular_file(destination));
    ASSERT_EQ(run({"catalogue", "info", "--path", "new/nested/catalogue.bin"}),
              0)
        << errors.str();
    EXPECT_NE(output.str().find("Catalogue: " + destination.string()),
              std::string::npos);
    EXPECT_EQ(run({"view", "--path", "new/nested/catalogue.bin"}), 0)
        << errors.str();
    EXPECT_TRUE(shown);
    EXPECT_EQ(read(root / "settings.toml"), saved);
    EXPECT_FALSE(std::filesystem::exists(root / "catalogue.bin"));
    no_temporary_files();
}

namespace
{
    struct Expected
    {
        std::uint64_t hip;
        double ra, dec, g;
    };
    // First-order tangent propagation from the retained native fixture to
    // J2016. 1 mas bounds its second-order difference (<0.7 mas here) from the
    // vector model.
    const std::array expected{
        Expected{14576, 47.04223615245371, 40.95563929255805, 2.0742011124304},
        Expected{25930, 83.00166990498822, -0.29909817807106526, 2.185029},
        Expected{58001, 178.45850544156627, 53.694808663426066,
                 2.3868610055232},
        Expected{24608, 79.17280939143004, 45.99609417878517,
                 -0.10273946410000001},
        Expected{27989, 88.79306242071242, 7.407114219562891,
                 -0.5850545279999999},
        Expected{24436, 78.63447295373072, -8.201636146345413, 0.1813293414},
        Expected{32349, 101.28462166488833, -16.721551668894765,
                 -1.1151784320607998}};
    void
    expect_landmarks(const std::vector<msc::catalogue::CatalogueStar>& records)
    {
        for (const auto& e : expected)
        {
            const auto found =
                std::ranges::find_if(records,
                                     [&](const auto& r)
                                     {
                                         return r.source_catalogue() ==
                                             CatalogueSource::Hipparcos2 &&
                                             r.source_id() == e.hip;
                                     });
            ASSERT_NE(found, records.end()) << e.hip;
            EXPECT_DOUBLE_EQ(found->star().magnitude().gaia_g(), e.g);
            EXPECT_DOUBLE_EQ(found->star().reference_epoch().julian_year(),
                             2016);
            EXPECT_NEAR(found->star()
                            .direction()
                            .get_right_ascension()
                            .get_angle()
                            .to_degrees(),
                        e.ra, 1.0 / 3600000);
            EXPECT_NEAR(found->star()
                            .direction()
                            .get_declination()
                            .get_angle()
                            .to_degrees(),
                        e.dec, 1.0 / 3600000);
        }
    }
} // namespace

TEST_F(CatalogueCommandTest, RealBrightHipparcosLandmarksReachPreparedBinary)
{
    sources(100, 0);
    const auto fixture =
        std::filesystem::path{__FILE__}.parent_path().parent_path() /
        "fixtures/hipparcos2/prepared.csv.gz";
    std::filesystem::copy_file(
        fixture, root / "hipparcos.csv.gz",
        std::filesystem::copy_options::overwrite_existing);
    ASSERT_EQ(run({"catalogue", "build"}), 0) << errors.str();
    const auto records = read_binary_catalogue(root / "catalogue.bin", 64);
    ASSERT_TRUE(records);
    ASSERT_EQ(records->size(), 64);
    expect_landmarks(*records);
    const auto bytes = read(root / "catalogue.bin");
    ASSERT_EQ(run({"catalogue", "build", "--force"}), 0) << errors.str();
    EXPECT_EQ(read(root / "catalogue.bin"), bytes);
}

TEST_F(CatalogueCommandTest,
       PublishedGaiaPreferenceNeedsActualSelectedMembership)
{
    for (const auto link : {1, 99})
    {
        sources(100, 2);
        gzip(root / "hipparcos.csv.gz",
             "hip_id,ref_epoch,ra,dec,pmra,pmdec,parallax,hp_mag,b_v,v_i,"
             "estimated_g,g_estimation_method,resolution_reason,gaia_source_id,"
             "excluded_gaia_source_id\n"
             "14576,1991.25,47,41,0,0,1,2,0,0,2.07,method,reason,0,0\n"
             "58001,1991.25,178,54,0,0,1,2,0,0,2.39,method,reason," +
                 std::to_string(link) + ",0\n");
        ASSERT_EQ(run({"catalogue", "build", "--force"}), 0) << errors.str();
        const auto records = read_binary_catalogue(root / "catalogue.bin");
        ASSERT_TRUE(records);
        ASSERT_EQ(records->size(), 64);
        EXPECT_EQ(std::ranges::count(
                      *records, CatalogueSource::Hipparcos2,
                      &msc::catalogue::CatalogueStar::source_catalogue),
                  link == 1 ? 1 : 2);
        EXPECT_EQ(std::ranges::count(*records, static_cast<std::uint64_t>(link),
                                     &msc::catalogue::CatalogueStar::source_id),
                  link == 1 ? 1 : 0);
    }
}

TEST_F(CatalogueCommandTest,
       FaintPublishedNeighbourDoesNotReplaceOrDuplicatePrimary)
{
    sources(100, 0);
    gzip(root / "hipparcos.csv.gz",
         "hip_id,ref_epoch,ra,dec,pmra,pmdec,parallax,hp_mag,b_v,v_i,"
         "estimated_g,g_estimation_method,resolution_reason,gaia_source_id,"
         "excluded_gaia_source_id\n"
         "25930,1991.25,83,-0.3,0,0,1,2.1,0,0,2.185029,method,published_gaia_"
         "outside_bright_range,0,7\n");
    ASSERT_EQ(run({"catalogue", "build"}), 0) << errors.str();
    const auto records = read_binary_catalogue(root / "catalogue.bin");
    ASSERT_TRUE(records);
    ASSERT_EQ(records->size(), 64);
    EXPECT_EQ(records->at(2).source_id(), 25930);
    EXPECT_DOUBLE_EQ(records->at(2).star().magnitude().gaia_g(), 2.185029);
    EXPECT_EQ(records->at(2).source_catalogue(), CatalogueSource::Hipparcos2);
    EXPECT_EQ(records->back().source_id(), 64);
    EXPECT_EQ(std::ranges::count(*records, 7,
                                 &msc::catalogue::CatalogueStar::source_id),
              0);
}
