#include "appearance_trial_private.hh"
#include "appearance_trial_shaders.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace msc::render::trial
{
    namespace
    {
        struct Instance
        {
            std::array<float, 3> enu{};
            float magnitude{};
            core::astronomy::LinearRgb8 colour;
            core::appearance::AnimationKey key;
        };

        GLuint shader(GLenum type, const char* source, std::string& error)
        {
            const auto result = glCreateShader(type);
            glShaderSource(result, 1, &source, nullptr);
            glCompileShader(result);
            GLint ok{};
            glGetShaderiv(result, GL_COMPILE_STATUS, &ok);
            if (ok == GL_TRUE)
                return result;
            GLint length{};
            glGetShaderiv(result, GL_INFO_LOG_LENGTH, &length);
            error.resize(static_cast<std::size_t>(std::max(length, 1)));
            glGetShaderInfoLog(result, length, nullptr, error.data());
            glDeleteShader(result);
            return 0;
        }

        bool valid_star(const TrialStar& star)
        {
            const std::array xyz{star.enu.x(), star.enu.y(), star.enu.z()};
            return std::isfinite(star.magnitude) &&
                std::ranges::all_of(
                       xyz, [](double v)
                       { return std::isfinite(v) && std::abs(v) <= 1.00001; });
        }

        std::vector<Instance> instances(std::span<const TrialStar> stars,
                                        std::uint32_t seed)
        {
            std::vector<Instance> result;
            result.reserve(stars.size());
            const auto salt =
                seed == 0 ? 0U : core::appearance::hash_noise({seed, 0}, 0);
            for (const auto& star : stars)
            {
                auto key = star.animation_key;
                key.low ^= salt;
                result.push_back({{static_cast<float>(star.enu.x()),
                                   static_cast<float>(star.enu.y()),
                                   static_cast<float>(star.enu.z())},
                                  static_cast<float>(
                                      std::clamp(star.magnitude, -30.0, 31.0)),
                                  star.colour,
                                  key});
            }
            return result;
        }

        void attribute(GLuint index, GLint size, GLenum type,
                       std::size_t offset)
        {
            glVertexAttribPointer(index, size, type, type == GL_UNSIGNED_BYTE,
                                  sizeof(Instance),
                                  reinterpret_cast<const void*>(offset));
            glEnableVertexAttribArray(index);
            glVertexAttribDivisor(index, 1);
        }

        void upload(Resources& resources, std::span<const TrialStar> stars,
                    std::uint32_t seed)
        {
            const auto data = instances(stars, seed);
            glGenVertexArrays(1, &resources.vao);
            glGenBuffers(1, &resources.buffer);
            glBindVertexArray(resources.vao);
            glBindBuffer(GL_ARRAY_BUFFER, resources.buffer);
            glBufferData(
                GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(data.size() * sizeof(Instance)),
                data.data(), GL_STATIC_DRAW);
            attribute(0, 3, GL_FLOAT, offsetof(Instance, enu));
            attribute(1, 1, GL_FLOAT, offsetof(Instance, magnitude));
            attribute(2, 3, GL_UNSIGNED_BYTE, offsetof(Instance, colour));
            glVertexAttribIPointer(
                3, 2, GL_UNSIGNED_INT, sizeof(Instance),
                reinterpret_cast<const void*>(offsetof(Instance, key)));
            glEnableVertexAttribArray(3);
            glVertexAttribDivisor(3, 1);
            resources.count = static_cast<GLsizei>(stars.size());
        }
    } // namespace

    Window::~Window()
    {
        if (handle != nullptr)
            glfwDestroyWindow(handle);
        glfwTerminate();
    }

    Resources::~Resources()
    {
        glDeleteTextures(1, &texture);
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(stars);
        glDeleteProgram(display);
    }

    GLuint compile_program(const char* vertex, const char* fragment,
                           std::string& error)
    {
        const auto vs = shader(GL_VERTEX_SHADER, vertex, error);
        if (vs == 0)
            return 0;
        const auto fs = shader(GL_FRAGMENT_SHADER, fragment, error);
        if (fs == 0)
        {
            glDeleteShader(vs);
            return 0;
        }
        const auto result = glCreateProgram();
        glAttachShader(result, vs);
        glAttachShader(result, fs);
        glLinkProgram(result);
        glDeleteShader(vs);
        glDeleteShader(fs);
        GLint ok{};
        glGetProgramiv(result, GL_LINK_STATUS, &ok);
        if (ok == GL_TRUE)
            return result;
        GLint length{};
        glGetProgramiv(result, GL_INFO_LOG_LENGTH, &length);
        error.resize(static_cast<std::size_t>(std::max(length, 1)));
        glGetProgramInfoLog(result, length, nullptr, error.data());
        glDeleteProgram(result);
        return 0;
    }

    std::expected<void, std::string> initialize(Window& window,
                                                const TrialSettings& settings)
    {
        if (glfwInit() != GLFW_TRUE)
            return std::unexpected{"GLFW initialization failed"};
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window.handle = glfwCreateWindow(settings.width, settings.height,
                                         "MyStarCeiling appearance trial",
                                         nullptr, nullptr);
        if (window.handle == nullptr)
            return std::unexpected{"Could not create appearance trial window"};
        glfwMakeContextCurrent(window.handle);
        glfwSwapInterval(1);
        if (gladLoadGL(glfwGetProcAddress) == 0)
            return std::unexpected{"Could not load OpenGL 3.3"};
        return {};
    }

    std::expected<void, std::string>
    initialize(Resources& resources, std::span<const TrialStar> stars,
               const TrialSettings& settings)
    {
        if (stars.size() >
                static_cast<std::size_t>(std::numeric_limits<GLsizei>::max()) ||
            !std::ranges::all_of(stars, valid_star))
            return std::unexpected{"Invalid appearance trial stars"};
        std::string error;
        resources.stars = compile_program(star_vertex, star_fragment, error);
        if (resources.stars == 0)
            return std::unexpected{"Star shader failure: " + error};
        resources.display =
            compile_program(display_vertex, display_fragment, error);
        if (resources.display == 0)
            return std::unexpected{"Display shader failure: " + error};
        upload(resources, stars, settings.seed);
        initialize_rays(resources.stars, settings);
        glGenFramebuffers(1, &resources.framebuffer);
        glGenTextures(1, &resources.texture);
        glDisable(GL_FRAMEBUFFER_SRGB);
        if (glGetError() != GL_NO_ERROR)
            return std::unexpected{"OpenGL appearance trial allocation failed"};
        return {};
    }

    std::expected<void, std::string> resize(Resources& resources, int width,
                                            int height)
    {
        if (resources.width == width && resources.height == height)
            return {};
        glBindTexture(GL_TEXTURE_2D, resources.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
                     GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, resources.framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, resources.texture, 0);
        if (glGetError() != GL_NO_ERROR ||
            glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            return std::unexpected{
                "OpenGL floating framebuffer allocation failed"};
        resources.width = width;
        resources.height = height;
        return {};
    }
} // namespace msc::render::trial
