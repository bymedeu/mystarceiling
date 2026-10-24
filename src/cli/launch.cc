#include "launch.hh"

#include <CLI/CLI.hpp>
#include <cstdlib>
#include <exception>
#include <functional>
#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <ostream>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

#include "config.hh"
#include "logging.hh"
#include "m1_scene.hh"

namespace msc::cli
{
    M1Scene prepare_view(const Settings& settings)
    {
        const auto stars = catalogue::read_binary_catalogue(settings.catalogue,
                                                            settings.quantity);
        if (!stars)
            throw std::runtime_error("Binary catalogue read failed: " +
                                     stars.error().detail);
        return make_m1_scene(*stars);
    }

    namespace
    {
        void add_options(CLI::App& app, CLI::App& view, Overrides& overrides)
        {
            app.add_option("--config", overrides.config,
                           "TOML configuration path");
            app.add_option("--log-file", overrides.log_file,
                           "Log file (empty uses stderr)");
            app.add_option("--log-level", overrides.log_level,
                           "trace/debug/info/warning/error/critical/off");
            view.add_option("--catalogue", overrides.catalogue,
                            "Prepared binary catalogue");
            view.add_option("--quantity", overrides.quantity,
                            "Catalogue prefix quantity (64..count)");
        }

        void report_scene(const M1Scene& scene)
        {
            spdlog::info(
                "Propagation failures: {}; visible records excluded by "
                "temporary G<=4 cutoff: {}; projected {} stars.",
                scene.propagation_failures, scene.magnitude_excluded,
                scene.points.size());
        }
    } // namespace

    int run_cli(int argc, const char* const* argv, std::ostream& output,
                std::ostream& errors,
                const std::function<void(const M1Scene&)>& show)
    {
        CLI::App app{"MyStarCeiling prepared-catalogue viewer", "msc"};
        app.set_version_flag("--version", MSC_PROJECT_VERSION);
        app.require_subcommand(1);
        auto* view = app.add_subcommand("view", "Display the star ceiling");
        view->fallthrough();
        Overrides overrides;
        add_options(app, *view, overrides);
        try
        {
            if (argc == 1)
            {
                output << app.help();
                return EXIT_SUCCESS;
            }
            app.parse(argc, argv);
            const auto settings = load_settings(overrides);
            const ScopedLogging logging{settings.log_file, settings.log_level};
            const auto scene = prepare_view(settings);
            report_scene(scene);
            show(scene);
            return EXIT_SUCCESS;
        }
        catch (const CLI::ParseError& error)
        {
            return app.exit(error, output, errors);
        }
        catch (const std::exception& error)
        {
            errors << "msc: " << error.what() << '\n';
            return EXIT_FAILURE;
        }
    }
} // namespace msc::cli
