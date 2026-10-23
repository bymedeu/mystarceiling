#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <span>

#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/catalogue/gaia_csv.hh>
#include <mystarceiling/catalogue/hipparcos_csv.hh>
#include <mystarceiling/catalogue/select_brightest.hh>
#include <mystarceiling/render/opengl_renderer.hh>
#include <optional>
#include <utility>
#include <vector>
#include "cli/m1_scene.hh"

using msc::catalogue::CatalogueSource;
using msc::catalogue::load_gaia_csv_gzip;
using msc::catalogue::load_hipparcos_csv_gzip;
using msc::catalogue::read_binary_catalogue;
using msc::catalogue::select_brightest;
using msc::catalogue::write_binary_catalogue;
using msc::cli::make_m1_scene;
using msc::render::show_star_window;

namespace
{
    constexpr std::size_t star_count = 4096;
    using msc::catalogue::CatalogueStar;

    void report_catalogues(const msc::catalogue::GaiaLoadResult& gaia,
                           const msc::catalogue::HipparcosLoadResult& hipparcos,
                           const std::vector<CatalogueStar>& catalogue)
    {
        std::cout << "Accepted " << gaia.statistics.accepted
                  << " Gaia rows (rejected " << gaia.statistics.rejected_total()
                  << "); " << hipparcos.statistics.accepted
                  << " Hipparcos rows (rejected "
                  << hipparcos.statistics.rejected_total() << "); selected "
                  << catalogue.size() << " total, including "
                  << std::count_if(catalogue.begin(), catalogue.end(),
                                   [](const auto& star)
                                   {
                                       return star.source_catalogue() ==
                                           CatalogueSource::Hipparcos2;
                                   })
                  << " Hipparcos stars.\n";
    }

    void report_scene(const msc::cli::M1Scene& scene)
    {
        std::cout << "Propagation failures: " << scene.propagation_failures
                  << "; visible records excluded by temporary G<=4 cutoff: "
                  << scene.magnitude_excluded << "; projected "
                  << scene.points.size() << " stars.\n";
    }

    std::optional<std::vector<CatalogueStar>>
    store_and_reload(const std::filesystem::path& runtime_path,
                     std::span<const CatalogueStar> selected)
    {
        const auto write_result =
            write_binary_catalogue(runtime_path, selected);
        if (!write_result)
        {
            std::cerr << "Binary catalogue write failed: "
                      << write_result.error().detail << '\n';
            return std::nullopt;
        }
        const auto stars = read_binary_catalogue(runtime_path);
        if (!stars)
        {
            std::cerr << "Binary catalogue read failed: "
                      << stars.error().detail << '\n';
            return std::nullopt;
        }

        return *stars;
    }
} // namespace

int run_scene(char** argv)
{
    const std::filesystem::path source_path{argv[1]};
    const std::filesystem::path runtime_path{argv[3]};
    const auto parsed = load_gaia_csv_gzip(source_path, star_count);
    if (!parsed)
    {
        std::cerr << "Gaia catalogue load failed: " << parsed.error().detail
                  << '\n';
        return EXIT_FAILURE;
    }

    const auto supplement = load_hipparcos_csv_gzip(argv[2]);
    if (!supplement)
    {
        std::cerr << "Hipparcos load failed: " << supplement.error().detail
                  << '\n';
        return EXIT_FAILURE;
    }
    auto combined = parsed->stars;
    combined.insert(combined.end(), supplement->stars.begin(),
                    supplement->stars.end());
    const auto selected = select_brightest(std::move(combined), star_count);
    const auto stars = store_and_reload(runtime_path, selected);
    if (!stars)
        return EXIT_FAILURE;

    const auto scene = make_m1_scene(*stars);
    report_catalogues(*parsed, *supplement, *stars);
    report_scene(scene);

    const auto render_result = show_star_window(std::span{scene.points});
    if (!render_result)
    {
        std::cerr << "Rendering failed: " << render_result.error() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <gaia.csv.gz> <hipparcos-supplement.csv.gz> "
                     "<runtime-catalogue.bin>\n";
        return EXIT_FAILURE;
    }

    return run_scene(argv);
}
