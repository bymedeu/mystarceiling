#pragma once

#include <array>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <string>
#include <vector>

namespace msc::catalogue
{
    enum class GaiaRowError
    {
        WrongColumnCount,
        MissingValue,
        InvalidSourceId,
        InvalidNumber,
        InvalidRightAscension,
        InvalidDeclination,
        Count
    };

    struct GaiaLoadStatistics
    {
        std::size_t rows_seen{};
        std::size_t accepted{};
        std::array<std::size_t, static_cast<std::size_t>(GaiaRowError::Count)>
            rejected{};

        [[nodiscard]] constexpr std::size_t rejected_total() const noexcept
        {
            return rows_seen - accepted;
        }
    };

    struct GaiaLoadResult
    {
        std::vector<CatalogueStar> stars;
        GaiaLoadStatistics statistics;
    };

    enum class GaiaFileErrorCode
    {
        OpenFailed,
        ReadFailed,
        InvalidHeader
    };

    struct GaiaFileError
    {
        GaiaFileErrorCode code;
        std::string detail;
    };

    [[nodiscard]] std::expected<GaiaLoadResult, GaiaFileError>
    load_gaia_csv_gzip(const std::filesystem::path& filename,
                       std::size_t maximum_accepted_rows);
} // namespace msc::catalogue
