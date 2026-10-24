add_library(mystarceiling_m1 m1_scene.cc)
target_include_directories(mystarceiling_m1 PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(mystarceiling_m1 PUBLIC mystarceiling_catalogue mystarceiling_core)

find_package(CLI11 CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(mystarceiling_launch STATIC config.cc launch.cc logging.cc)
target_include_directories(mystarceiling_launch PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(mystarceiling_launch PUBLIC mystarceiling_m1 spdlog::spdlog
    PRIVATE CLI11::CLI11 tomlplusplus::tomlplusplus)
target_compile_definitions(mystarceiling_launch PRIVATE
    MSC_PROJECT_VERSION="${PROJECT_VERSION}")

add_executable(mystarceiling_cli main.cc)
set_target_properties(mystarceiling_cli PROPERTIES OUTPUT_NAME msc)
target_link_libraries(mystarceiling_cli PRIVATE mystarceiling_launch
    mystarceiling_renderer_gl)
