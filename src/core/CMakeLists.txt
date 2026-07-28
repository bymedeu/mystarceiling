add_library(mystarceiling_core
            foo.cc)

target_compile_options(
    mystarceiling_core PRIVATE  -Wall -Wextra -Werror
)

target_include_directories(mystarceiling_core
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
