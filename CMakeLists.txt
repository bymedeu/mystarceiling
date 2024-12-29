cmake_minimum_required(VERSION 3.10)

# Project name
project(StarMapping)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(StarMapping
    src/main.cpp
    src/data_collector.cpp
    src/csv_parser.cpp
    src/coordinate_converter.cpp
    src/renderer.cpp
    src/time_simulation.cpp
    src/utils.cpp
)

# Include directories
target_include_directories(StarMapping PUBLIC include)

# Link libraries (for OpenGL and dependencies)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)

target_link_libraries(StarMapping PRIVATE OpenGL::GL GLEW::GLEW GLFW::GLFW gl