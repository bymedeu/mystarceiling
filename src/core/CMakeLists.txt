add_library(mystarceiling_core
            foo.cc
            coordinates/icrs_direction.cc
            coordinates/icrs_to_enu.cc
            time/earth_rotation.cc
)

target_compile_options(
    mystarceiling_core PRIVATE  -Wall -Wextra
)

# GCC 16 requires an explicit flag for C++26 reflection, including in headers.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
   CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
    target_compile_options(mystarceiling_core PUBLIC -freflection)
endif()

target_include_directories(mystarceiling_core
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
