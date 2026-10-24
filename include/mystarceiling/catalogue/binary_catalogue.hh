#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <mystarceiling/catalogue/catalogue_star.hh>
#include <span>
#include <string>
#include <vector>

namespace msc::catalogue
{
    enum class BinaryCatalogueErrorCode
    {
        OpenFailed,
        ReadFailed,
        WriteFailed,
        InvalidMagic,
        UnsupportedVersion,
        UnsupportedSchema,
        InvalidRecordSize,
        InvalidFileSize,
        InvalidRecord,
        InvalidQuantity
    };

    struct BinaryCatalogueError
    {
        BinaryCatalogueErrorCode code{};
        std::string detail;
    };

    struct BinaryCatalogueInfo
    {
        std::uint64_t records{};
        std::uint64_t file_size{};
    };

    // Checks the header and total file length without decoding records.
    [[nodiscard]] std::expected<BinaryCatalogueInfo, BinaryCatalogueError>
    read_binary_catalogue_info(const std::filesystem::path& filename);

    [[nodiscard]] std::expected<void, BinaryCatalogueError>
    write_binary_catalogue(const std::filesystem::path& filename,
                           std::span<const CatalogueStar> stars);

    [[nodiscard]] std::expected<std::vector<CatalogueStar>,
                                BinaryCatalogueError>
    read_binary_catalogue(const std::filesystem::path& filename);
    [[nodiscard]] std::expected<std::vector<CatalogueStar>,
                                BinaryCatalogueError>
    read_binary_catalogue(const std::filesystem::path& filename,
                          std::size_t quantity);
} // namespace msc::catalogue
