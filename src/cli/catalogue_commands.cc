#include "catalogue_commands.hh"

#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iomanip>
#include <mystarceiling/catalogue/binary_catalogue.hh>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/catalogue/gaia_csv.hh>
#include <mystarceiling/catalogue/hipparcos_csv.hh>
#include <mystarceiling/catalogue/select_brightest.hh>
#include <ostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <system_error>
#include <unistd.h>
#include <unordered_set>
#include <utility>
#include <vector>

namespace msc::cli
{
    using catalogue::CatalogueStar;
    namespace
    {
        std::filesystem::path canonical_path(const std::filesystem::path& path)
        {
            std::error_code error;
            const auto canonical =
                std::filesystem::weakly_canonical(path, error);
            return error ? path.lexically_normal() : canonical;
        }

        bool same_file(const std::filesystem::path& a,
                       const std::filesystem::path& b)
        {
            if (a.empty() || b.empty())
                return false;
            if (canonical_path(a) == canonical_path(b))
                return true;
            std::error_code error;
            return std::filesystem::equivalent(a, b, error) && !error;
        }

        class TemporaryCatalogue
        {
        public:
            explicit TemporaryCatalogue(const std::filesystem::path& target)
            {
                auto pattern =
                    (target.parent_path() / ".msc-catalogue-XXXXXX").string();
                const int descriptor = ::mkstemp(pattern.data());
                if (descriptor < 0)
                    throw std::system_error(
                        errno, std::generic_category(),
                        "Cannot create temporary catalogue");
                path_ = pattern;
                if (::close(descriptor) != 0)
                {
                    const auto error = errno;
                    std::error_code ignored;
                    std::filesystem::remove(path_, ignored);
                    throw std::system_error(error, std::generic_category(),
                                            "Cannot close temporary catalogue");
                }
            }
            ~TemporaryCatalogue()
            {
                std::error_code ignored;
                std::filesystem::remove(path_, ignored);
            }
            TemporaryCatalogue(const TemporaryCatalogue&) = delete;
            TemporaryCatalogue& operator=(const TemporaryCatalogue&) = delete;
            [[nodiscard]] const std::filesystem::path& path() const
            {
                return path_;
            }

        private:
            std::filesystem::path path_;
        };

        void warn_rejected(std::string_view source, std::size_t rejected)
        {
            if (rejected != 0)
                spdlog::warn("{}: skipped {} unusable rows", source, rejected);
        }

        std::vector<CatalogueStar>
        load_gaia_records(const std::filesystem::path& path,
                          std::size_t remaining,
                          const std::unordered_set<std::uint64_t>& excluded)
        {
            if (remaining >
                std::vector<CatalogueStar>{}.max_size() - excluded.size())
                throw std::runtime_error(
                    "Gaia prefix size exceeds vector capacity");
            auto gaia = catalogue::load_gaia_csv_gzip(
                path, remaining + excluded.size());
            if (!gaia)
                throw std::runtime_error("Gaia load failed: " +
                                         gaia.error().detail);
            warn_rejected("Gaia", gaia->statistics.rejected_total());
            std::erase_if(gaia->stars, [&](const auto& star)
                          { return excluded.contains(star.source_id()); });
            if (gaia->stars.size() > remaining)
                gaia->stars.erase(gaia->stars.begin() + remaining,
                                  gaia->stars.end());
            return std::move(gaia->stars);
        }

        std::vector<CatalogueStar> select_records(const Settings& settings)
        {
            auto hipparcos =
                catalogue::load_hipparcos_csv_gzip(settings.hipparcos);
            if (!hipparcos)
                throw std::runtime_error("Hipparcos load failed: " +
                                         hipparcos.error().detail);
            warn_rejected("Hipparcos", hipparcos->statistics.rejected_total());
            const auto required =
                hipparcos->stars.size() - hipparcos->gaia_preferences.size();
            const auto remaining =
                settings.capacity - std::min(settings.capacity, required);
            std::vector<CatalogueStar> gaia_records;
            if (remaining != 0)
                gaia_records = load_gaia_records(settings.gaia, remaining,
                                                 hipparcos->excluded_gaia_ids);
            auto combined = catalogue::resolve_hipparcos_first(
                std::move(hipparcos->stars), hipparcos->gaia_preferences,
                std::move(gaia_records), settings.capacity);
            if (combined.size() != settings.capacity)
                throw InsufficientStars{settings.capacity, combined.size()};
            return combined;
        }

        void verify_catalogue(const std::filesystem::path& path,
                              std::size_t count)
        {
            const auto records = catalogue::read_binary_catalogue(path);
            if (!records)
                throw std::runtime_error(
                    "Built catalogue verification failed: " +
                    records.error().detail);
            if (records->size() != count ||
                !std::ranges::is_sorted(*records, catalogue::brightness_less))
                throw std::runtime_error("Built catalogue count or brightness "
                                         "ordering is incorrect");
        }

        std::string gb_size(std::uint64_t bytes)
        {
            const auto gb = static_cast<long double>(bytes) / 1000000000.0L;
            int precision = 3;
            if (gb > 0 && gb < 0.001L)
                precision = 2 - static_cast<int>(std::floor(std::log10(gb)));
            std::ostringstream text;
            text << std::fixed << std::setprecision(precision) << gb;
            return text.str();
        }

        std::string shell_quote(std::string_view value)
        {
            std::string result{"'"};
            for (const auto ch : value)
                result += ch == '\'' ? "'\\''" : std::string(1, ch);
            return result + "'";
        }
    } // namespace

    InsufficientStars::InsufficientStars(std::size_t requested,
                                         std::size_t available) :
        std::runtime_error("Requested " + std::to_string(requested) +
                           " records; available " + std::to_string(available)),
        available_{available}
    {}

    void check_build_paths(const Settings& settings, bool force)
    {
        if (settings.capacity < 64 ||
            settings.capacity > std::vector<CatalogueStar>{}.max_size())
            throw std::runtime_error("Invalid catalogue.capacity: require at "
                                     "least 64 and at most vector capacity");
        for (const auto& source :
             {settings.gaia, settings.hipparcos, settings.config_file})
            if (same_file(settings.catalogue, source))
                throw std::runtime_error(
                    "Catalogue destination must not overwrite an input source "
                    "or configuration");
        if (!force &&
            std::filesystem::symlink_status(settings.catalogue).type() !=
                std::filesystem::file_type::not_found)
            throw std::runtime_error(
                "Catalogue already exists; use --force to replace it");
        if (force && std::filesystem::is_directory(settings.catalogue))
            throw std::runtime_error("Catalogue destination is a directory");
    }

    void check_log_paths(const Settings& settings)
    {
        for (const auto& protected_path :
             {settings.catalogue, settings.gaia, settings.hipparcos,
              settings.config_file})
            if (same_file(settings.log_file, protected_path))
                throw std::runtime_error("Log file must not overwrite a "
                                         "catalogue, source or configuration");
    }

    void build_catalogue(const Settings& settings, bool force,
                         std::ostream& output)
    {
        check_build_paths(settings, force);
        const auto selected = select_records(settings);
        std::filesystem::create_directories(settings.catalogue.parent_path());
        const TemporaryCatalogue temporary{settings.catalogue};
        const auto written =
            catalogue::write_binary_catalogue(temporary.path(), selected);
        if (!written)
            throw std::runtime_error("Binary catalogue write failed: " +
                                     written.error().detail);
        verify_catalogue(temporary.path(), settings.capacity);
        if (force)
            std::filesystem::rename(temporary.path(), settings.catalogue);
        else
            std::filesystem::create_hard_link(temporary.path(),
                                              settings.catalogue);
        output << "Built catalogue: " << settings.catalogue.string()
               << "\nRecords: " << selected.size() << '\n';
    }

    void catalogue_info(const Settings& settings, std::ostream& output)
    {
        const auto info =
            catalogue::read_binary_catalogue_info(settings.catalogue);
        if (!info)
            throw std::runtime_error("Binary catalogue inspection failed: " +
                                     info.error().detail);
        output << "Catalogue: " << settings.catalogue.string()
               << "\nRecords: " << info->records
               << "\nFile size: " << info->file_size << " bytes ("
               << gb_size(info->file_size) << " GB)\n";
    }

    void suggest_capacity(int argc, const char* const* argv,
                          std::size_t available, std::ostream& errors)
    {
        if (available < 64)
            return;
        errors << "Retry: ";
        for (int i = 0; i < argc; ++i)
        {
            const std::string_view argument{argv[i]};
            if (argument == "--capacity")
            {
                ++i;
                continue;
            }
            if (argument.starts_with("--capacity="))
                continue;
            errors << shell_quote(argument) << ' ';
        }
        errors << "--capacity " << available << '\n';
    }
} // namespace msc::cli
