cmake_minimum_required(VERSION 3.25)

project(MyStarCeiling
        VERSION 0.0.1
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MSC_ENABLE_SANITIZERS "Enable first-party AddressSanitizer and UBSan" OFF)
if(MSC_ENABLE_SANITIZERS AND NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
    message(FATAL_ERROR "MSC_ENABLE_SANITIZERS requires GCC or Clang")
endif()

if(NOT MSC_ENABLE_SANITIZERS)
    add_custom_target(copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()

option(MSC_ENABLE_TESTS "Enable testing" ON)

add_subdirectory(src)

if(MSC_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Apply project checks only to first-party targets, never fetched dependencies.
option(MSC_WARNINGS_AS_ERRORS "Treat first-party warnings as errors" OFF)
foreach(msc_target IN ITEMS mystarceiling_core mystarceiling_catalogue
            mystarceiling_m1 mystarceiling_cli mystarceiling_renderer_gl
            mystarceiling_tests)
    if(TARGET ${msc_target})
        if(MSC_WARNINGS_AS_ERRORS)
            target_compile_options(${msc_target} PRIVATE
                "$<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:-Wall;-Wextra;-Werror>")
        endif()
        if(MSC_ENABLE_SANITIZERS)
            target_compile_options(${msc_target} PRIVATE
                -fsanitize=address,undefined -fno-sanitize-recover=all
                -fno-omit-frame-pointer -g)
            target_link_options(${msc_target} PRIVATE
                -fsanitize=address,undefined -fno-sanitize-recover=all)
        endif()
    endif()
endforeach()
