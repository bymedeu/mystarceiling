#pragma once

#include <cstddef>
#include <iosfwd>
#include <stdexcept>

#include "config.hh"

namespace msc::cli
{
    class InsufficientStars : public std::runtime_error
    {
    public:
        InsufficientStars(std::size_t requested, std::size_t available);
        [[nodiscard]] std::size_t available() const noexcept
        {
            return available_;
        }

    private:
        std::size_t available_;
    };

    void check_build_paths(const Settings& settings, bool force);
    void check_log_paths(const Settings& settings);
    void build_catalogue(const Settings& settings, bool force,
                         std::ostream& output);
    void catalogue_info(const Settings& settings, std::ostream& output);
    void suggest_capacity(int argc, const char* const* argv,
                          std::size_t available, std::ostream& errors);
} // namespace msc::cli
