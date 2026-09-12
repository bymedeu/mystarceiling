#include <mystarceiling/render/opengl_renderer.hh>

#include <cstddef>
#include <expected>
#include <span>
#include <string>

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace msc::render {
namespace {
constexpr int window_width = 1280;
constexpr int window_height = 720;

GLuint compile_shader(GLenum type, const char *source, std::string &error) {
  const GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint compiled = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_TRUE)
    return shader;

  GLint log_length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  error.resize(static_cast<std::size_t>(log_length));
  glGetShaderInfoLog(shader, log_length, nullptr, error.data());
  glDeleteShader(shader);
  return 0;
}

GLuint create_program(std::string &error) {
  constexpr const char *vertex_source = R"(
                #version 330 core
                layout (location = 0) in vec2 position;
                layout (location = 1) in float intensity;
                out float star_intensity;
                void main()
                {
                    gl_Position = vec4(position, 0.0, 1.0);
                    gl_PointSize = 3.0;
                    star_intensity = intensity;
                }
            )";
  constexpr const char *fragment_source = R"(
                #version 330 core
                in float star_intensity;
                out vec4 colour;
                void main()
                {
                    colour = vec4(vec3(star_intensity), 1.0);
                }
            )";

  const GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_source, error);
  if (vertex == 0)
    return 0;
  const GLuint fragment =
      compile_shader(GL_FRAGMENT_SHADER, fragment_source, error);
  if (fragment == 0) {
    glDeleteShader(vertex);
    return 0;
  }

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  GLint linked = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (linked == GL_TRUE)
    return program;

  GLint log_length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
  error.resize(static_cast<std::size_t>(log_length));
  glGetProgramInfoLog(program, log_length, nullptr, error.data());
  glDeleteProgram(program);
  return 0;
}
} // namespace

std::expected<void, std::string>
show_star_window(std::span<const ScreenPoint> points) {
  if (glfwInit() != GLFW_TRUE)
    return std::unexpected{"GLFW initialization failed"};

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window = glfwCreateWindow(window_width, window_height,
                                        "MyStarCeiling M1", nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return std::unexpected{"Could not create the GLFW window"};
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  if (gladLoadGL(glfwGetProcAddress) == 0) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return std::unexpected{"Could not load OpenGL 3.3"};
  }

  std::string shader_error;
  const GLuint program = create_program(shader_error);
  if (program == 0) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return std::unexpected{"OpenGL shader failure: " + shader_error};
  }

  GLuint vertex_array = 0;
  GLuint vertex_buffer = 0;
  glGenVertexArrays(1, &vertex_array);
  glGenBuffers(1, &vertex_buffer);
  glBindVertexArray(vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(points.size_bytes()),
               points.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenPoint), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      1, 1, GL_FLOAT, GL_FALSE, sizeof(ScreenPoint),
      reinterpret_cast<const void *>(offsetof(ScreenPoint, intensity)));
  glEnableVertexAttribArray(1);
  glEnable(GL_PROGRAM_POINT_SIZE);

  while (glfwWindowShouldClose(window) == GLFW_FALSE) {
    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);
    glClearColor(0.005F, 0.008F, 0.02F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(points.size()));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vertex_buffer);
  glDeleteVertexArrays(1, &vertex_array);
  glDeleteProgram(program);
  glfwDestroyWindow(window);
  glfwTerminate();
  return {};
}
} // namespace msc::render
