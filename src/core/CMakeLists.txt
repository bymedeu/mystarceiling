add_library(mystarceiling_core
            foo.cc
            coordinates/icrs_direction.cc
            coordinates/icrs_to_enu.cc
            time/earth_rotation.cc
)

target_compile_options(
    mystarceiling_core PRIVATE  -Wall -Wextra
)

target_include_directories(mystarceiling_core
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
