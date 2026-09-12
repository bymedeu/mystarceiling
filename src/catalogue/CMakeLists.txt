find_package(ZLIB REQUIRED)
find_package(spdlog REQUIRED)

add_library(mystarceiling_catalogue
    gaia_csv.cc
    binary_catalogue.cc
)

target_compile_options(mystarceiling_catalogue PRIVATE -Wall -Wextra)
target_include_directories(mystarceiling_catalogue
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(mystarceiling_catalogue
    PUBLIC mystarceiling_core
    PRIVATE ZLIB::ZLIB spdlog::spdlog
)
