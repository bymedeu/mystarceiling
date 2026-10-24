#pragma once

#include <cstddef>
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
