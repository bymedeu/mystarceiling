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

#include "catalogue_commands.hh"
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
        struct Commands
        {
            CLI::App* build{};
            CLI::App* info{};
            bool force{};
        };

        void add_options(CLI::App& app, CLI::App& view, Overrides& overrides,
                         Commands& commands)
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
            auto* catalogue = app.add_subcommand(
                "catalogue", "Prepare or inspect a catalogue");
            catalogue->require_subcommand(1);
            catalogue->fallthrough();
            commands.build = catalogue->add_subcommand(
                "build", "Build a Hipparcos-first catalogue");
            commands.info = catalogue->add_subcommand(
                "info", "Show catalogue path, record count and size");
            commands.build->fallthrough();
            commands.info->fallthrough();
            commands.build->add_option("--gaia", overrides.gaia,
                                       "Ordered Gaia gzip CSV");
            commands.build->add_option("--hipparcos", overrides.hipparcos,
                                       "Prepared Hipparcos supplement");
            commands.build->add_option(
                "--capacity", overrides.capacity,
                "Exact stored record count (at least 64)");
            commands.build->add_option("--catalogue", overrides.catalogue,
                                       "Destination binary catalogue");
            commands.build->add_flag("--force", commands.force,
                                     "Replace an existing catalogue");
            commands.info->add_option("--catalogue", overrides.catalogue,
                                      "Prepared binary catalogue");
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

    namespace
    {
        void execute(const Commands& commands, const Settings& settings,
                     std::ostream& output,
                     const std::function<void(const M1Scene&)>& show)
        {
            if (commands.build->parsed())
                check_build_paths(settings, commands.force);
            check_log_paths(settings);
            const ScopedLogging logging{settings.log_file, settings.log_level};
            if (commands.build->parsed())
                build_catalogue(settings, commands.force, output);
            else if (commands.info->parsed())
                catalogue_info(settings, output);
            else
            {
                const auto scene = prepare_view(settings);
                report_scene(scene);
                show(scene);
            }
        }
    } // namespace

    int run_cli(int argc, const char* const* argv, std::ostream& output,
                std::ostream& errors,
                const std::function<void(const M1Scene&)>& show)
    {
        CLI::App app{"MyStarCeiling catalogue tools and viewer", "msc"};
        app.set_version_flag("--version", MSC_PROJECT_VERSION);
        app.require_subcommand(1);
        auto* view = app.add_subcommand("view", "Display the star ceiling");
        view->fallthrough();
        Overrides overrides;
        Commands commands{};
        add_options(app, *view, overrides, commands);
        try
        {
            if (argc == 1)
            {
                output << app.help();
                return EXIT_SUCCESS;
            }
            app.parse(argc, argv);
            execute(commands, load_settings(overrides), output, show);
            return EXIT_SUCCESS;
        }
        catch (const CLI::ParseError& error)
        {
            return app.exit(error, output, errors);
        }
        catch (const InsufficientStars& error)
        {
            errors << "msc: " << error.what() << '\n';
            suggest_capacity(argc, argv, error.available(), errors);
        }
        catch (const std::exception& error)
        {
            errors << "msc: " << error.what() << '\n';
        }
        return EXIT_FAILURE;
    }
} // namespace msc::cli
