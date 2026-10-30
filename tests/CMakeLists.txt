find_package(GTest CONFIG REQUIRED)
find_package(ZLIB CONFIG REQUIRED)

add_executable(mystarceiling_tests
            render/appearance_settings_tests.cc
            catalogue/binary_catalogue_tests.cc
            catalogue/gaia_csv_tests.cc
            catalogue/stellar_colour_tests.cc
            catalogue/hipparcos_csv_tests.cc
            catalogue/select_brightest_tests.cc
            cli/m1_scene_tests.cc
            cli/catalogue_commands_tests.cc
            cli/config_tests.cc
            cli/launch_tests.cc
            cli/logging_tests.cc
            core/appearance/scintillation_tests.cc
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
    PRIVATE mystarceiling_launch mystarceiling_m1 mystarceiling_core mystarceiling_catalogue GTest::gtest_main ZLIB::ZLIB)
if(NOT MSC_ENABLE_SANITIZERS)
    set_target_properties(mystarceiling_tests PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

include(GoogleTest)
gtest_discover_tests(mystarceiling_tests)

option(MSC_ENABLE_OPENGL_TESTS "Enable developer headless OpenGL regression tests" OFF)
if(MSC_ENABLE_OPENGL_TESTS)
    add_executable(mystarceiling_opengl_tests
        render/opengl_test_context.cc
        render/opengl_star_light_tests.cc
        render/opengl_pixels_tests.cc)
    target_include_directories(mystarceiling_opengl_tests
        PRIVATE ${PROJECT_SOURCE_DIR}/src/render)
    target_compile_definitions(mystarceiling_opengl_tests PRIVATE GLFW_INCLUDE_NONE)
    target_link_libraries(mystarceiling_opengl_tests PRIVATE
        mystarceiling_renderer_gl mystarceiling_core glfw msc_glad GTest::gtest_main)
    gtest_discover_tests(mystarceiling_opengl_tests PROPERTIES LABELS opengl)
endif()
