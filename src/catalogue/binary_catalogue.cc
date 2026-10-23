#include <mystarceiling/catalogue/binary_catalogue.hh>

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <fstream>
#include <ios>
#include <istream>
#include <limits>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <mystarceiling/core/astronomy/star.hh>
#include <mystarceiling/core/astronomy/stellar_measurements.hh>
#include <mystarceiling/core/coordinates/declination.hh>
#include <mystarceiling/core/coordinates/icrs_direction.hh>
#include <mystarceiling/core/coordinates/proper_motion.hh>
#include <mystarceiling/core/coordinates/right_ascension.hh>
#include <mystarceiling/core/maths/angle.hh>
#include <mystarceiling/core/time/reference_epoch.hh>
#include <optional>
#include <ostream>
#include <span>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

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
        using u16 = std::uint16_t;
        using u32 = std::uint32_t;
        using u64 = std::uint64_t;

        constexpr std::array<char, 8> magic{'M', 'S', 'C', 'S',
                                            'T', 'A', 'R', '\0'};
        constexpr u16 format_version = 2;
        constexpr u16 header_size = 32;
        constexpr u32 schema_id = 2;
        constexpr u32 record_size = 74;

        template <typename Integer>
        void write_unsigned(std::ostream& stream, Integer value)
        {
            static_assert(std::is_unsigned_v<Integer>);
            for (std::size_t index = 0; index < sizeof(Integer); ++index)
            {
                stream.put(static_cast<char>(value & 0xffU));
                value >>= 8U;
            }
        }

        void write_double(std::ostream& stream, double value)
        {
            static_assert(sizeof(double) == sizeof(u64));
            static_assert(std::numeric_limits<double>::is_iec559);
            write_unsigned(stream, std::bit_cast<u64>(value));
        }

        template <typename Integer>
        bool read_unsigned(std::istream& stream, Integer& value)
        {
            static_assert(std::is_unsigned_v<Integer>);
            value = 0;
            for (std::size_t index = 0; index < sizeof(Integer); ++index)
            {
                const int byte = stream.get();
                if (byte == std::char_traits<char>::eof())
                    return false;
                value |= static_cast<Integer>(static_cast<unsigned char>(byte))
                    << (index * 8U);
            }
            return true;
        }

        bool read_double(std::istream& stream, double& value)
        {
            u64 bits{};
            if (!read_unsigned(stream, bits))
                return false;
            value = std::bit_cast<double>(bits);
            return true;
        }

        BinaryCatalogueError error(BinaryCatalogueErrorCode code,
                                   std::string detail)
        {
            return BinaryCatalogueError{code, std::move(detail)};
        }
        void write_header(std::ostream& output, std::size_t count)
        {
            output.write(magic.data(),
                         static_cast<std::streamsize>(magic.size()));
            write_unsigned(output, format_version);
            write_unsigned(output, header_size);
            write_unsigned(output, schema_id);
            write_unsigned(output, record_size);
            write_unsigned(output, u32{0});
            write_unsigned(output, static_cast<u64>(count));
        }

        void write_record(std::ostream& output, const CatalogueStar& entry)
        {
            const auto& star = entry.star();
            write_unsigned(output, entry.source_id());
            write_double(output, star.reference_epoch().julian_year());
            write_double(output,
                         star.direction()
                             .get_right_ascension()
                             .get_angle()
                             .to_degrees());
            write_double(
                output,
                star.direction().get_declination().get_angle().to_degrees());
            write_double(output,
                         star.proper_motion()
                             .right_ascension_cos_declination
                             .milliarcseconds_per_year());
            write_double(
                output,
                star.proper_motion().declination.milliarcseconds_per_year());
            write_double(output, star.parallax().milliarcseconds());
            write_double(output, star.magnitude().gaia_g());
            write_double(output,
                         star.colour() ? star.colour()->gaia_bp_minus_rp()
                                       : 0.0);
            write_unsigned(output,
                           static_cast<std::uint8_t>(entry.source_catalogue()));
            write_unsigned(
                output, static_cast<std::uint8_t>(star.colour().has_value()));
        }

        std::expected<void, BinaryCatalogueError>
        read_magic_bytes(std::istream& input,
                         const std::filesystem::path& filename)
        {
            std::array<char, magic.size()> read_magic{};
            input.read(read_magic.data(),
                       static_cast<std::streamsize>(read_magic.size()));
            if (read_magic != magic)
                return std::unexpected{error(
                    BinaryCatalogueErrorCode::InvalidMagic, filename.string())};
            return {};
        }

        std::expected<void, BinaryCatalogueError>
        validate_header(u16 version, u16 actual_header_size, u32 actual_schema,
                        u32 actual_record_size)
        {
            if (version != format_version || actual_header_size != header_size)
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::UnsupportedVersion,
                          "Unsupported catalogue header or version")};
            if (actual_schema != schema_id)
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::UnsupportedSchema,
                          "Unsupported stellar schema")};
            if (actual_record_size != record_size)
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::InvalidRecordSize,
                          "Unexpected stellar record size")};
            return {};
        }

        std::expected<u64, BinaryCatalogueError>
        read_header(std::istream& input)
        {
            u16 version{};
            u16 actual_header_size{};
            u32 actual_schema{};
            u32 actual_record_size{};
            u32 reserved{};
            u64 count{};
            if (!read_unsigned(input, version) ||
                !read_unsigned(input, actual_header_size) ||
                !read_unsigned(input, actual_schema) ||
                !read_unsigned(input, actual_record_size) ||
                !read_unsigned(input, reserved) || !read_unsigned(input, count))
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::ReadFailed,
                          "Truncated catalogue header")};
            const auto validation = validate_header(
                version, actual_header_size, actual_schema, actual_record_size);
            if (!validation)
                return std::unexpected{validation.error()};
            return count;
        }

        std::expected<void, BinaryCatalogueError>
        validate_file_size(const std::filesystem::path& filename, u64 count)
        {
            std::error_code size_error;
            const auto actual_file_size =
                std::filesystem::file_size(filename, size_error);
            if (size_error ||
                count > (std::numeric_limits<u64>::max() - header_size) /
                        record_size ||
                actual_file_size != header_size + count * record_size)
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::InvalidFileSize,
                          "File length does not match row count")};
            if (count > std::vector<CatalogueStar>{}.max_size())
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::InvalidFileSize,
                          "Row count exceeds vector capacity")};

            return {};
        }

        struct Record
        {
            u64 source_id{};
            std::array<double, 8> values{};
            std::uint8_t provenance{};
            std::uint8_t has_colour{};
        };

        std::expected<Record, BinaryCatalogueError>
        read_record(std::istream& input)
        {
            Record record;
            auto& source_id = record.source_id;
            auto& values = record.values;
            auto& provenance = record.provenance;
            auto& has_colour = record.has_colour;
            if (!read_unsigned(input, source_id))
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::ReadFailed,
                          "Truncated stellar record")};
            bool complete = true;
            for (auto& value : values)
                complete = complete && read_double(input, value);
            complete = complete && read_unsigned(input, provenance) &&
                read_unsigned(input, has_colour);
            if (!complete)
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::ReadFailed,
                          "Truncated stellar record")};

            return record;
        }

        std::expected<Star, BinaryCatalogueError>
        decode_star(const Record& record)
        {
            const auto& values = record.values;
            const auto has_colour = record.has_colour;
            const auto epoch = ReferenceEpoch::try_from_julian_year(values[0]);
            const auto ra =
                RightAscension::try_from(Angle::from_degrees(values[1]));
            const auto dec =
                Declination::try_from(Angle::from_degrees(values[2]));
            const auto pmra =
                ProperMotion::try_from_milliarcseconds_per_year(values[3]);
            const auto pmdec =
                ProperMotion::try_from_milliarcseconds_per_year(values[4]);
            const auto parallax = Parallax::try_from_milliarcseconds(values[5]);
            const auto magnitude =
                ApparentMagnitude::try_from_gaia_g(values[6]);
            const auto colour =
                ColourIndex::try_from_gaia_bp_minus_rp(values[7]);
            if (!epoch || !ra || !dec || !pmra || !pmdec || !parallax ||
                !magnitude || !colour)
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::InvalidRecord,
                          "Invalid stellar record")};
            return Star{
                *epoch,
                IcrsDirection{*ra, *dec},
                ProperMotionComponents{.right_ascension_cos_declination = *pmra,
                                       .declination = *pmdec},
                *parallax,
                *magnitude,
                has_colour ? std::optional{*colour} : std::nullopt};
        }

        std::expected<CatalogueStar, BinaryCatalogueError>
        decode_record(const Record& record)
        {
            const auto& [source_id, values, provenance, has_colour] = record;
            const auto star = decode_star(record);
            if (!star)
                return std::unexpected{star.error()};
            if (source_id == 0 || has_colour > 1 ||
                provenance >
                    static_cast<std::uint8_t>(CatalogueSource::Hipparcos2) ||
                (!has_colour && values[7] != 0.0))
                return std::unexpected{
                    error(BinaryCatalogueErrorCode::InvalidRecord,
                          "Invalid stellar record")};
            return CatalogueStar{
                source_id, static_cast<CatalogueSource>(provenance), *star};
        }

    } // namespace

    std::expected<void, BinaryCatalogueError>
    write_binary_catalogue(const std::filesystem::path& filename,
                           std::span<const CatalogueStar> stars)
    {
        std::ofstream output{filename, std::ios::binary | std::ios::trunc};
        if (!output)
            return std::unexpected{
                error(BinaryCatalogueErrorCode::OpenFailed, filename.string())};
        write_header(output, stars.size());
        for (const auto& entry : stars)
            write_record(output, entry);

        if (!output)
            return std::unexpected{error(BinaryCatalogueErrorCode::WriteFailed,
                                         filename.string())};
        return {};
    }

    std::expected<std::vector<CatalogueStar>, BinaryCatalogueError>
    read_binary_catalogue(const std::filesystem::path& filename)
    {
        std::ifstream input{filename, std::ios::binary};
        if (!input)
            return std::unexpected{
                error(BinaryCatalogueErrorCode::OpenFailed, filename.string())};

        const auto magic_result = read_magic_bytes(input, filename);
        if (!magic_result)
            return std::unexpected{magic_result.error()};
        const auto header = read_header(input);
        if (!header)
            return std::unexpected{header.error()};
        const auto count = *header;
        const auto size_result = validate_file_size(filename, count);
        if (!size_result)
            return std::unexpected{size_result.error()};

        std::vector<CatalogueStar> stars;
        stars.reserve(static_cast<std::size_t>(count));
        for (u64 index = 0; index < count; ++index)
        {
            const auto record = read_record(input);
            if (!record)
                return std::unexpected{record.error()};
            const auto star = decode_record(*record);
            if (!star)
                return std::unexpected{star.error()};
            stars.push_back(*star);
        }
        return stars;
    }
} // namespace msc::catalogue
