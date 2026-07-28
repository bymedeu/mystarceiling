add_executable(mystarceiling_cli
            main.cc)

target_link_libraries(mystarceiling_cli PRIVATE
                mystarceiling_core
                )
