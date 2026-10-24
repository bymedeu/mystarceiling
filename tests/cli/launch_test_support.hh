#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <stdlib.h>
#include <string>

namespace msc::cli::test
{
    class LaunchTest : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            char pattern[] = "/tmp/msc-view-XXXXXX";
            const auto* directory = ::mkdtemp(pattern);
            if (!directory)
                throw std::runtime_error(
                    "Cannot create isolated test directory");
            root = directory;
            cwd = std::filesystem::current_path();
            for (std::size_t i = 0; i < names.size(); ++i)
                if (const auto* value = std::getenv(names[i]))
                    environment[i] = value;
            setenv("HOME", (root / "home").c_str(), 1);
            setenv("XDG_CONFIG_HOME", (root / "config").c_str(), 1);
            setenv("XDG_DATA_HOME", (root / "data").c_str(), 1);
            std::filesystem::current_path(root);
        }

        void TearDown() override
        {
            std::filesystem::current_path(cwd);
            for (std::size_t i = 0; i < names.size(); ++i)
            {
                if (environment[i])
                    setenv(names[i], environment[i]->c_str(), 1);
                else
                    unsetenv(names[i]);
            }
            std::filesystem::remove_all(root);
        }

        static void write(const std::filesystem::path& path,
                          const std::string& text)
        {
            std::filesystem::create_directories(path.parent_path());
            std::ofstream{path} << text;
        }

        static std::string read(const std::filesystem::path& path)
        {
            std::ifstream stream{path};
            return {std::istreambuf_iterator<char>{stream}, {}};
        }

        std::filesystem::path config() const
        {
            return root / "config/mystarceiling/config.toml";
        }

        static std::string valid_config()
        {
            return "[catalogue]\npath = 'catalogue.bin'\n"
                   "[view]\nquantity = 4096\n"
                   "[logging]\nfile = ''\nlevel = 'warning'\n";
        }

        std::filesystem::path root;

    private:
        std::filesystem::path cwd;
        static constexpr std::array names{"HOME", "XDG_CONFIG_HOME",
                                          "XDG_DATA_HOME"};
        std::array<std::optional<std::string>, names.size()> environment;
    };
} // namespace msc::cli::test
