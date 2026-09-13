find_package(GTest REQUIRED)
find_package(ZLIB REQUIRED)
#find_package(fmt REQUIRED)

add_executable(mystarceiling_tests
            catalogue/binary_catalogue_tests.cc
            catalogue/gaia_csv_tests.cc
            catalogue/hipparcos_csv_tests.cc
            catalogue/select_brightest_tests.cc
            cli/m1_scene_tests.cc
            core/astronomy/stellar_measurements_tests.cc
            core/coordinates/declination_tests.cc
            core/coordinates/propagate_proper_motion_tests.cc
            core/coordinates/icrs_direction_tests.cc
            core/coordinates/icrs_to_enu_tests.cc
            core/coordinates/latitude_tests.cc
            core/coordinates/longitude_tests.cc
            core/coordinates/observer_location_tests.cc
            core/coordinates/right_ascension_tests.cc
            core/maths/angle_tests.cc
            core/maths/vector3_tests.cc
            core/projection/zenith_perspective_tests.cc
            core/time/earth_rotation_tests.cc
            core/time/julian_date_tests.cc
            integration/catalogue_to_screen_tests.cc
)
target_link_libraries(mystarceiling_tests
    PRIVATE mystarceiling_m1 mystarceiling_core mystarceiling_catalogue GTest::gtest_main ZLIB::ZLIB)
set_target_properties(mystarceiling_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(GoogleTest)
gtest_discover_tests(mystarceiling_tests)
