add_library(mystarceiling_m1 m1_scene.cc)
target_include_directories(mystarceiling_m1 PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(mystarceiling_m1 PUBLIC mystarceiling_catalogue mystarceiling_core)

add_executable(mystarceiling_cli
            main.cc)

target_link_libraries(mystarceiling_cli PRIVATE
                mystarceiling_m1
                mystarceiling_catalogue
                mystarceiling_core
                mystarceiling_renderer_gl
                )
