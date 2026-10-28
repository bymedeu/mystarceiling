#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace msc::catalogue
{
    enum class HipparcosRowError
    {
        WrongColumnCount,
        MissingValue,
        InvalidSourceId,
        InvalidNumber,
        InvalidRightAscension,
        InvalidDeclination,
        InvalidPropagation,
        Count
    };

    struct HipparcosLoadStatistics
    {
        std::size_t rows_seen{};
        std::size_t accepted{};
        std::array<std::size_t,
                   static_cast<std::size_t>(HipparcosRowError::Count)>
            rejected{};

        [[nodiscard]] constexpr std::size_t rejected_total() const noexcept
        {
            return rows_seen - accepted;
        }
    };

    struct HipparcosLoadResult
    {
        std::vector<CatalogueStar> stars;
        HipparcosLoadStatistics statistics;
        // Optional published, complete local Gaia links; native fallbacks stay
        // owned here.
        std::unordered_map<std::uint64_t, std::uint64_t> gaia_preferences;
        std::unordered_set<std::uint64_t> excluded_gaia_ids;
    };

    enum class HipparcosFileErrorCode
    {
        OpenFailed,
        ReadFailed,
        InvalidHeader
    };

    struct HipparcosFileError
    {
        HipparcosFileErrorCode code{};
        std::string detail;
    };

    [[nodiscard]] std::expected<HipparcosLoadResult, HipparcosFileError>
    load_hipparcos_csv_gzip(const std::filesystem::path& filename);
} // namespace msc::catalogue
