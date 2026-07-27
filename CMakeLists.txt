cmake_minimum_required(VERSION 3.23)

project(MyStarCeiling)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)

add_subdirectory(src)

if (GTEST_FOUND)
    add_executable(tests
                    tests/tests.cc)

    target_link_libraries(tests PUBLIC GTest)

    target_include_directories(tests PUBLIC include/)
endif()
