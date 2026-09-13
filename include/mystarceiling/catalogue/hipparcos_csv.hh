#pragma once

#include <array>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <mystarceiling/catalogue/star.hh>
#include <string>
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
        std::vector<Star> stars;
        HipparcosLoadStatistics statistics;
    };

    enum class HipparcosFileErrorCode
    {
        OpenFailed,
        ReadFailed,
        InvalidHeader
    };

    struct HipparcosFileError
    {
        HipparcosFileErrorCode code;
        std::string detail;
    };

    [[nodiscard]] std::expected<HipparcosLoadResult, HipparcosFileError>
    load_hipparcos_csv_gzip(const std::filesystem::path& filename);
} // namespace msc::catalogue
