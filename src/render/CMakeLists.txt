include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG d9d6f0f1f967807ffade6598ea9a631ebaf37a56
    GIT_SHALLOW FALSE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG 73db193f853e2ee079bf3ca8a64aa2eaf6459043
    GIT_SHALLOW FALSE
    SOURCE_SUBDIR cmake
)
FetchContent_MakeAvailable(glad)
glad_add_library(msc_glad STATIC REPRODUCIBLE API gl:core=3.3 EXTENSIONS NONE)

add_library(mystarceiling_renderer_gl opengl_renderer.cc)

target_include_directories(mystarceiling_renderer_gl
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_definitions(mystarceiling_renderer_gl PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(mystarceiling_renderer_gl PRIVATE glfw msc_glad)
