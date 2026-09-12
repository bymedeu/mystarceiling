cmake_minimum_required(VERSION 3.25)

project(MyStarCeiling
        VERSION 0.0.1
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

option(MSC_ENABLE_TESTS "Enable testing" ON)

add_subdirectory(src)

if(MSC_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
