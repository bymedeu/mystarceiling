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
#include <utility>
#include "cli/m1_scene.hh"

using msc::catalogue::CatalogueSource;
using msc::catalogue::load_gaia_csv_gzip;
using msc::catalogue::load_hipparcos_csv_gzip;
using msc::catalogue::read_binary_catalogue;
using msc::catalogue::select_brightest;
using msc::catalogue::write_binary_catalogue;
using msc::cli::make_m1_scene;
using msc::render::show_star_window;

int main(int argc, char** argv)
{
    constexpr std::size_t star_count = 4096;

    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <gaia.csv.gz> <hipparcos-supplement.csv.gz> "
                     "<runtime-catalogue.bin>\n";
        return EXIT_FAILURE;
    }

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
    const auto write_result = write_binary_catalogue(runtime_path, selected);
    if (!write_result)
    {
        std::cerr << "Binary catalogue write failed: "
                  << write_result.error().detail << '\n';
        return EXIT_FAILURE;
    }
    const auto stars = read_binary_catalogue(runtime_path);
    if (!stars)
    {
        std::cerr << "Binary catalogue read failed: " << stars.error().detail
                  << '\n';
        return EXIT_FAILURE;
    }

    const auto scene = make_m1_scene(*stars);
    std::cout << "Accepted " << parsed->statistics.accepted
              << " Gaia rows (rejected " << parsed->statistics.rejected_total()
              << "); " << supplement->statistics.accepted
              << " Hipparcos rows (rejected "
              << supplement->statistics.rejected_total() << "); selected "
              << stars->size() << " total, including "
              << std::count_if(stars->begin(), stars->end(),
                               [](const auto& star)
                               {
                                   return star.source_catalogue() ==
                                       CatalogueSource::Hipparcos2;
                               })
              << " Hipparcos stars.\n"
              << "Propagation failures: " << scene.propagation_failures
              << "; visible records excluded by temporary G<=4 cutoff: "
              << scene.magnitude_excluded << "; projected "
              << scene.points.size() << " stars.\n";

    const auto render_result = show_star_window(std::span{scene.points});
    if (!render_result)
    {
        std::cerr << "Rendering failed: " << render_result.error() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
