find_package(GTest REQUIRED)
#find_package(fmt REQUIRED)

add_executable(mystarceiling_tests
            tests.cc
            declination_tests.cc
            icrs_direction_tests.cc
            icrs_to_enu_tests.cc
            latitude_tests.cc
            longitude_tests.cc
            observer_location_tests.cc
            right_ascension_tests.cc
            angle_tests.cc
            vector3_tests.cc
)
target_link_libraries(mystarceiling_tests
    PRIVATE mystarceiling_core GTest::gtest_main)
set_target_properties(mystarceiling_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(GoogleTest)
gtest_discover_tests(mystarceiling_tests)
