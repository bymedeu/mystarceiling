add_executable(msc
            main.cc) 

target_compile_options(
    msc PUBLIC -std=g++23 -Wall -Wextra -Werror
)

target_include_directories(msc PUBLIC include/)
