#pragma once

#include <mystarceiling/render/appearance_trial.hh>

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <array>
#include <cstddef>
#include <expected>
#include <span>
#include <string>

namespace msc::render::trial
{
    struct Window
    {
        GLFWwindow* handle{};
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        ~Window();
    };

    struct Resources
    {
        GLuint stars{}, display{}, vao{}, buffer{}, framebuffer{}, texture{};
        int width{}, height{};
        GLsizei count{};
        Resources() = default;
        Resources(const Resources&) = delete;
        Resources& operator=(const Resources&) = delete;
        ~Resources();
    };

    struct State
    {
        TrialSettings settings;
        core::appearance::AnimationClock clock;
    };

    [[nodiscard]] std::expected<void, std::string>
    initialize(Window&, const TrialSettings&);
    [[nodiscard]] std::expected<void, std::string>
    initialize(Resources&, std::span<const TrialStar>, const TrialSettings&);
    [[nodiscard]] std::expected<void, std::string> resize(Resources&, int width,
                                                          int height);
    [[nodiscard]] std::expected<void, std::string> render(Resources&,
                                                          const State&);
    void install_controls(GLFWwindow*, State&);
    void update_title(GLFWwindow*, const State&);
    void uniform(GLuint program, const char* name, double value);
    void set_profile(GLuint program, const TrialSettings&);
    void initialize_rays(GLuint program, const TrialSettings&);
    [[nodiscard]] GLuint compile_program(const char* vertex,
                                         const char* fragment,
                                         std::string& error);
} // namespace msc::render::trial
