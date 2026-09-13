#include <mystarceiling/catalogue/hipparcos_csv.hh>

#include <array>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <memory>
#include <mystarceiling/catalogue/star.hh>
#include <mystarceiling/catalogue/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>
#include <zlib.h>

namespace msc::catalogue
{
    using core::coordinates::Declination;
    using core::coordinates::IcrsDirection;
    using core::coordinates::ProperMotion;
    using core::coordinates::ProperMotionComponents;
    using core::coordinates::RightAscension;
    using core::maths::Angle;
    using core::time::ReferenceEpoch;

    namespace
    {
        using u64 = std::uint64_t;

        constexpr std::string_view expected_header =
            "hip_id,ref_epoch,ra,dec,pmra,pmdec,parallax,hp_mag,b_v,v_i,"
            "estimated_g,g_estimation_method,resolution_reason";

        struct GzipCloser
        {
            void operator()(gzFile_s* file) const noexcept
            {
                if (file != nullptr)
                    gzclose(file);
            }
        };
        using GzipFile = std::unique_ptr<gzFile_s, GzipCloser>;

        std::string_view strip_line_ending(std::string_view line)
        {
            while (!line.empty() &&
                   (line.back() == '\n' || line.back() == '\r'))
                line.remove_suffix(1);
            return line;
        }

        std::array<std::string_view, 13> split_row(std::string_view line,
                                                   bool& valid)
        {
            std::array<std::string_view, 13> fields{};
            std::size_t begin = 0;
            for (std::size_t index = 0; index < fields.size(); ++index)
            {
                const auto end = line.find(',', begin);
                fields[index] = line.substr(begin, end - begin);
                if (end == std::string_view::npos)
                {
                    valid = index + 1 == fields.size();
                    return fields;
                }
                if (index + 1 == fields.size())
                {
                    valid = false;
                    return fields;
                }
                begin = end + 1;
            }
            valid = false;
            return fields;
        }

        template <typename Number>
        bool parse_number(std::string_view text, Number& value)
        {
            const auto [end, error] =
                std::from_chars(text.data(), text.data() + text.size(), value);
            return error == std::errc{} && end == text.data() + text.size();
        }

        void reject(HipparcosLoadStatistics& statistics,
                    HipparcosRowError error, std::size_t row_number)
        {
            ++statistics.rejected[static_cast<std::size_t>(error)];
            spdlog::debug("Rejected Hipparcos row {} with reason {}",
                          row_number, static_cast<unsigned>(error));
        }
    } // namespace

    std::expected<HipparcosLoadResult, HipparcosFileError>
    load_hipparcos_csv_gzip(const std::filesystem::path& filename)
    {
        GzipFile file{gzopen(filename.c_str(), "rb")};
        if (!file)
            return std::unexpected{HipparcosFileError{
                HipparcosFileErrorCode::OpenFailed, filename.string()}};

        std::array<char, 4096> buffer{};
        if (gzgets(file.get(), buffer.data(),
                   static_cast<int>(buffer.size())) == nullptr)
            return std::unexpected{
                HipparcosFileError{HipparcosFileErrorCode::ReadFailed,
                                   "Could not read Hipparcos header"}};
        if (strip_line_ending(buffer.data()) != expected_header)
            return std::unexpected{
                HipparcosFileError{HipparcosFileErrorCode::InvalidHeader,
                                   std::string{buffer.data()}}};

        HipparcosLoadResult result;
        while (gzgets(file.get(), buffer.data(),
                      static_cast<int>(buffer.size())) != nullptr)
        {
            ++result.statistics.rows_seen;
            const std::size_t row_number = result.statistics.rows_seen + 1;
            const auto line = strip_line_ending(buffer.data());
            bool valid_columns = false;
            const auto fields = split_row(line, valid_columns);
            if (!valid_columns)
            {
                reject(result.statistics, HipparcosRowError::WrongColumnCount,
                       row_number);
                continue;
            }
            bool missing = false;
            for (const auto field : fields)
                missing = missing || field.empty();
            if (missing)
            {
                reject(result.statistics, HipparcosRowError::MissingValue,
                       row_number);
                continue;
            }

            u64 source_id{};
            std::array<double, 10> values{};
            if (!parse_number(fields[0], source_id) || source_id == 0)
            {
                reject(result.statistics, HipparcosRowError::InvalidSourceId,
                       row_number);
                continue;
            }
            bool valid_numbers = true;
            for (std::size_t index = 0; index < values.size(); ++index)
                valid_numbers = valid_numbers &&
                    parse_number(fields[index + 1], values[index]) &&
                    std::isfinite(values[index]);
            if (!valid_numbers)
            {
                reject(result.statistics, HipparcosRowError::InvalidNumber,
                       row_number);
                continue;
            }

            const auto ra =
                RightAscension::try_from(Angle::from_degrees(values[1]));
            if (!ra)
            {
                reject(result.statistics,
                       HipparcosRowError::InvalidRightAscension, row_number);
                continue;
            }
            const auto dec =
                Declination::try_from(Angle::from_degrees(values[2]));
            if (!dec)
            {
                reject(result.statistics, HipparcosRowError::InvalidDeclination,
                       row_number);
                continue;
            }

            const auto epoch = ReferenceEpoch::try_from_julian_year(values[0]);
            const auto pmra =
                ProperMotion::try_from_milliarcseconds_per_year(values[3]);
            const auto pmdec =
                ProperMotion::try_from_milliarcseconds_per_year(values[4]);
            const auto parallax = Parallax::try_from_milliarcseconds(values[5]);
            const auto magnitude =
                ApparentMagnitude::try_from_gaia_g(values[9]);
            result.stars.emplace_back(
                source_id, *epoch, IcrsDirection{*ra, *dec},
                ProperMotionComponents{.right_ascension_cos_declination = *pmra,
                                       .declination = *pmdec},
                *parallax, *magnitude, std::nullopt,
                CatalogueSource::Hipparcos2);
            ++result.statistics.accepted;
        }

        int error_number = Z_OK;
        const char* error_text = gzerror(file.get(), &error_number);
        if (error_number != Z_OK && error_number != Z_STREAM_END)
            return std::unexpected{HipparcosFileError{
                HipparcosFileErrorCode::ReadFailed,
                error_text == nullptr ? "gzip read failure" : error_text}};
        return result;
    }
} // namespace msc::catalogue
