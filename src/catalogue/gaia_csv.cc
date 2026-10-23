#include <mystarceiling/catalogue/gaia_csv.hh>

#include <array>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <memory>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>
#include <zlib.h>

namespace msc::catalogue
{
    using core::astronomy::ApparentMagnitude;
    using core::astronomy::ColourIndex;
    using core::astronomy::Parallax;
    using core::astronomy::Star;
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
            "source_id,ref_epoch,ra,dec,pmra,pmdec,parallax,phot_g_mean_mag,bp_"
            "rp";

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

        bool split_row(std::string_view line,
                       std::array<std::string_view, 9>& fields)
        {
            std::size_t begin = 0;
            for (std::size_t index = 0; index < fields.size(); ++index)
            {
                const auto end = line.find(',', begin);
                fields[index] = line.substr(begin, end - begin);
                if (end == std::string_view::npos)
                    return index + 1 == fields.size();
                if (index + 1 == fields.size())
                    return false;
                begin = end + 1;
            }
            return false;
        }

        template <typename Number>
        bool parse_number(std::string_view text, Number& value)
        {
            const auto [end, error] =
                std::from_chars(text.data(), text.data() + text.size(), value);
            return error == std::errc{} && end == text.data() + text.size();
        }

        void reject(GaiaLoadStatistics& statistics, GaiaRowError error,
                    std::size_t row_number)
        {
            ++statistics.rejected[static_cast<std::size_t>(error)];
            spdlog::debug("Rejected Gaia row {} with reason {}", row_number,
                          static_cast<unsigned>(error));
        }
        bool parse_measurements(const std::array<std::string_view, 9>& fields,
                                std::array<double, 8>& values)
        {
            bool valid_numbers = true;
            for (std::size_t index = 0; index < values.size(); ++index)
                valid_numbers = valid_numbers &&
                    parse_number(fields[index + 1], values[index]) &&
                    std::isfinite(values[index]);
            return valid_numbers;
        }

        struct ParsedRow
        {
            u64 source_id{};
            std::array<double, 8> values{};
        };

        std::expected<ParsedRow, GaiaRowError>
        parse_row_numbers(std::string_view line)
        {
            std::array<std::string_view, 9> fields{};
            if (!split_row(line, fields))
            {
                return std::unexpected{GaiaRowError::WrongColumnCount};
            }
            bool missing = false;
            for (const auto field : fields)
                missing = missing || field.empty();
            if (missing)
            {
                return std::unexpected{GaiaRowError::MissingValue};
            }

            ParsedRow row;
            auto& source_id = row.source_id;
            auto& values = row.values;
            if (!parse_number(fields[0], source_id) || source_id == 0)
            {
                return std::unexpected{GaiaRowError::InvalidSourceId};
            }
            const bool valid_numbers = parse_measurements(fields, values);
            if (!valid_numbers)
            {
                return std::unexpected{GaiaRowError::InvalidNumber};
            }

            return row;
        }

        std::expected<CatalogueStar, GaiaRowError>
        parse_star(const ParsedRow& row)
        {
            const auto source_id = row.source_id;
            const auto& values = row.values;
            const auto ra =
                RightAscension::try_from(Angle::from_degrees(values[1]));
            if (!ra)
            {
                return std::unexpected{GaiaRowError::InvalidRightAscension};
            }
            const auto dec =
                Declination::try_from(Angle::from_degrees(values[2]));
            if (!dec)
            {
                return std::unexpected{GaiaRowError::InvalidDeclination};
            }

            const auto epoch = ReferenceEpoch::try_from_julian_year(values[0]);
            const auto pmra =
                ProperMotion::try_from_milliarcseconds_per_year(values[3]);
            const auto pmdec =
                ProperMotion::try_from_milliarcseconds_per_year(values[4]);
            const auto parallax = Parallax::try_from_milliarcseconds(values[5]);
            const auto magnitude =
                ApparentMagnitude::try_from_gaia_g(values[6]);
            const auto colour =
                ColourIndex::try_from_gaia_bp_minus_rp(values[7]);
            return CatalogueStar{
                source_id, CatalogueSource::GaiaDr3,
                Star{*epoch, IcrsDirection{*ra, *dec},
                     ProperMotionComponents{.right_ascension_cos_declination =
                                                *pmra,
                                            .declination = *pmdec},
                     *parallax, *magnitude, *colour}};
        }

        std::expected<void, GaiaFileError>
        read_header(gzFile file, std::array<char, 4096>& buffer)
        {
            if (gzgets(file, buffer.data(), static_cast<int>(buffer.size())) ==
                nullptr)
                return std::unexpected{
                    GaiaFileError{GaiaFileErrorCode::ReadFailed,
                                  "Could not read Gaia header"}};
            if (strip_line_ending(buffer.data()) != expected_header)
                return std::unexpected{
                    GaiaFileError{GaiaFileErrorCode::InvalidHeader,
                                  std::string{buffer.data()}}};

            return {};
        }
        std::expected<void, GaiaFileError> gzip_status(gzFile file)
        {
            int error_number = Z_OK;
            const char* error_text = gzerror(file, &error_number);
            if (error_number != Z_OK && error_number != Z_STREAM_END)
                return std::unexpected{GaiaFileError{
                    GaiaFileErrorCode::ReadFailed,
                    error_text == nullptr ? "gzip read failure" : error_text}};
            return {};
        }
        void accept_row(std::string_view line, GaiaLoadResult& result)
        {
            ++result.statistics.rows_seen;
            const std::size_t row_number = result.statistics.rows_seen + 1;
            const auto numbers = parse_row_numbers(line);
            const auto star = numbers
                ? parse_star(*numbers)
                : std::expected<CatalogueStar, GaiaRowError>{
                      std::unexpected{numbers.error()}};
            if (!star)
            {
                reject(result.statistics, star.error(), row_number);
                return;
            }
            result.stars.push_back(*star);
            ++result.statistics.accepted;
        }
    } // namespace

    std::expected<GaiaLoadResult, GaiaFileError>
    load_gaia_csv_gzip(const std::filesystem::path& filename,
                       std::size_t maximum_accepted_rows)
    {
        GzipFile file{gzopen(filename.c_str(), "rb")};
        if (!file)
            return std::unexpected{GaiaFileError{GaiaFileErrorCode::OpenFailed,
                                                 filename.string()}};

        std::array<char, 4096> buffer{};
        const auto header = read_header(file.get(), buffer);
        if (!header)
            return std::unexpected{header.error()};

        GaiaLoadResult result;
        result.stars.reserve(maximum_accepted_rows);
        while (result.statistics.accepted < maximum_accepted_rows &&
               gzgets(file.get(), buffer.data(),
                      static_cast<int>(buffer.size())) != nullptr)
        {
            accept_row(strip_line_ending(buffer.data()), result);
        }

        const auto status = gzip_status(file.get());
        if (!status)
            return std::unexpected{status.error()};
        return result;
    }
} // namespace msc::catalogue
