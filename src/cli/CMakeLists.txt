add_executable(mystarceiling_cli
            main.cc)

target_link_libraries(mystarceiling_cli PRIVATE
                mystarceiling_catalogue
                mystarceiling_core
                mystarceiling_renderer_gl
                )
