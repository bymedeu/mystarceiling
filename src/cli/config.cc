#include "config.hh"
#include <cstddef>

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <toml.hpp>
#include <utility>

#include "logging.hh"

namespace msc::cli
{
    namespace
    {
        std::filesystem::path xdg_root(const char* variable,
                                       const char* fallback)
        {
            const auto* value = std::getenv(variable);
            if (value && std::filesystem::path{value}.is_absolute())
                return value;
            const auto* home = std::getenv("HOME");
            if (!home || !std::filesystem::path{home}.is_absolute())
                throw std::runtime_error(
                    std::string{variable} +
                    " needs an absolute path or absolute HOME");
            return std::filesystem::path{home} / fallback;
        }

        std::filesystem::path resolve(const std::filesystem::path& base,
                                      const std::string& value)
        {
            return (base / value).lexically_normal();
        }

        void check_keys(const toml::table& table, std::string_view section,
                        std::initializer_list<std::string_view> allowed)
        {
            for (const auto& [key, node] : table)
                if (std::ranges::find(allowed, key.str()) == allowed.end())
                    throw std::runtime_error(
                        "Unknown setting: " + std::string{section} +
                        std::string{key.str()});
        }

        void check_section(const toml::table& table, std::string_view section,
                           std::initializer_list<std::string_view> allowed)
        {
            const auto* child = table[section].as_table();
            if (!child)
                throw std::runtime_error("Missing or wrong type: " +
                                         std::string{section});
            check_keys(*child, std::string{section} + ".", allowed);
        }

        std::string required_string(const toml::table& table,
                                    std::string_view section,
                                    std::string_view key)
        {
            const auto value = table[section][key].value<std::string>();
            if (!value)
                throw std::runtime_error(
                    "Missing or wrong type: " + std::string{section} + "." +
                    std::string{key});
            return *value;
        }

        toml::table parse_config(const std::filesystem::path& path)
        {
            try
            {
                return toml::parse_file(path.string());
            }
            catch (const toml::parse_error& error)
            {
                std::ostringstream message;
                message << "Invalid config " << path << ": " << error;
                throw std::runtime_error(message.str());
            }
        }

        std::size_t required_count(const toml::table& table,
                                   std::string_view section,
                                   std::string_view key)
        {
            const auto node = table[section][key];
            const auto name = std::string{section} + "." + std::string{key};
            if (!node.is_integer())
                throw std::runtime_error("Missing or wrong type: " + name);
            const auto number = *node.value<std::int64_t>();
            if (number < 0 || !std::in_range<std::size_t>(number))
                throw std::runtime_error("Invalid " + name + ": out of range");
            return static_cast<std::size_t>(number);
        }

        Settings read_settings(const std::filesystem::path& path)
        {
            const auto table = parse_config(path);
            check_keys(table, "", {"catalogue", "sources", "view", "logging"});
            check_section(table, "catalogue", {"path", "capacity"});
            check_section(table, "view", {"quantity"});
            check_section(table, "logging", {"file", "level"});
            check_section(table, "sources", {"gaia", "hipparcos"});
            const auto base = path.parent_path();
            Settings settings;
            settings.catalogue =
                resolve(base, required_string(table, "catalogue", "path"));
            settings.quantity = required_count(table, "view", "quantity");
            settings.capacity = required_count(table, "catalogue", "capacity");
            settings.gaia =
                resolve(base, required_string(table, "sources", "gaia"));
            settings.hipparcos =
                resolve(base, required_string(table, "sources", "hipparcos"));
            const auto file = required_string(table, "logging", "file");
            settings.log_file =
                file.empty() ? std::filesystem::path{} : resolve(base, file);
            settings.log_level = required_string(table, "logging", "level");
            settings.config_file = path;
            logging_level(settings.log_level);
            return settings;
        }

        void create_defaults(const std::filesystem::path& path)
        {
            const auto data =
                xdg_root("XDG_DATA_HOME", ".local/share") / "mystarceiling";
            const toml::table table{
                {"catalogue",
                 toml::table{{"path", (data / "catalogue.bin").string()},
                             {"capacity", 4096}}},
                {"sources",
                 toml::table{
                     {"gaia", (data / "sources/gaia-dr3.csv.gz").string()},
                     {"hipparcos",
                      (data / "sources/hipparcos2-supplement.csv.gz")
                          .string()}}},
                {"view", toml::table{{"quantity", 4096}}},
                {"logging",
                 toml::table{{"file", "/tmp/mystarceiling.log"},
                             {"level", "warning"}}}};
            std::filesystem::create_directories(path.parent_path());
            std::ofstream output{path, std::ios::out | std::ios::noreplace};
            if (!output)
            {
                if (std::filesystem::exists(path))
                    return; // A concurrent creator won; read and validate its
                            // file.
                throw std::runtime_error("Cannot create config: " +
                                         path.string());
            }
            output << table << '\n';
            output.close();
            if (!output)
                throw std::runtime_error("Cannot write config: " +
                                         path.string());
        }

        std::size_t cli_count(const std::string& text, std::string_view setting)
        {
            std::size_t number{};
            const auto [end, error] =
                std::from_chars(text.data(), text.data() + text.size(), number);
            if (error != std::errc{} || end != text.data() + text.size())
                throw std::runtime_error("Invalid " + std::string{setting} +
                                         ": expected unsigned integer");
            return number;
        }

        void apply_overrides(Settings& settings, const Overrides& overrides)
        {
            const auto cwd = std::filesystem::current_path();
            if (overrides.catalogue)
                settings.catalogue = resolve(cwd, *overrides.catalogue);
            if (overrides.quantity)
                settings.quantity =
                    cli_count(*overrides.quantity, "view.quantity");
            if (overrides.gaia)
                settings.gaia = resolve(cwd, *overrides.gaia);
            if (overrides.hipparcos)
                settings.hipparcos = resolve(cwd, *overrides.hipparcos);
            if (overrides.capacity)
                settings.capacity =
                    cli_count(*overrides.capacity, "catalogue.capacity");
            if (overrides.log_file)
                settings.log_file = overrides.log_file->empty()
                    ? std::filesystem::path{}
                    : resolve(cwd, *overrides.log_file);
            if (overrides.log_level)
                settings.log_level = *overrides.log_level;
            logging_level(settings.log_level);
        }
    } // namespace

    Settings load_settings(const Overrides& overrides)
    {
        if (overrides.config && overrides.config->empty())
            throw std::runtime_error("Invalid --config: empty path");
        const auto path = overrides.config
            ? std::filesystem::absolute(*overrides.config)
            : xdg_root("XDG_CONFIG_HOME", ".config") /
                "mystarceiling/config.toml";
        if (!std::filesystem::exists(path))
            create_defaults(path);
        auto settings = read_settings(path);
        apply_overrides(settings, overrides);
        return settings;
    }
} // namespace msc::cli
