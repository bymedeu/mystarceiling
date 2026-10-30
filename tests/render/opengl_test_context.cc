#include "opengl_test_context.hh"

#include "appearance_trial_private.hh"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <expected>
#include <span>
#include <string>

#include <mystarceiling/render/appearance_trial.hh>

namespace msc::render::test
{
    namespace
    {
        std::string provider_error()
        {
            const char* description{};
            const auto code = glfwGetError(&description);
            return std::to_string(code) + ": " +
                (description != nullptr ? description : "no provider detail");
        }
    } // namespace

    std::expected<void, std::string> OpenGlContext::open()
    {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
        if (glfwInit() != GLFW_TRUE)
            return std::unexpected{"GLFW NULL provider unavailable: " +
                                   provider_error()};
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        // The fixed EGL pbuffer also accommodates the bounded resize test.
        window.handle =
            glfwCreateWindow(96, 80, "MSC GL tests", nullptr, nullptr);
        if (window.handle == nullptr)
            return std::unexpected{"NULL/EGL OpenGL 3.3 unavailable: " +
                                   provider_error()};
        glfwMakeContextCurrent(window.handle);
        if (glfwGetCurrentContext() != window.handle)
            return std::unexpected{"EGL current context unavailable: " +
                                   provider_error()};
        if (gladLoadGL(glfwGetProcAddress) == 0 || !GLAD_GL_VERSION_3_3)
            return std::unexpected{
                "EGL provider lacks OpenGL 3.3 entry points"};
        glReadBuffer(GL_FRONT);
        state.settings.width = 64;
        state.settings.height = 64;
        trial::install_controls(window.handle, state);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        return {};
    }

    void OpenGlContext::initialize_scene(std::span<const TrialStar> stars)
    {
        resources.reset();
        auto& target = resources.emplace();
        const auto initialized =
            trial::initialize(target, stars, state.settings);
        ASSERT_TRUE(initialized) << initialized.error();
        const auto resized =
            trial::resize(target, state.settings.width, state.settings.height);
        ASSERT_TRUE(resized) << resized.error();
    }

    void OpenGlContext::draw()
    {
        const auto rendered = trial::render(*resources, state);
        ASSERT_TRUE(rendered) << rendered.error();
    }

    void OpenGlContext::toggle_picture()
    {
        // Exercise the installed production Space handler without a display or
        // synthesizing platform events. Restore it before invoking it.
        const auto callback = glfwSetKeyCallback(window.handle, nullptr);
        ASSERT_NE(callback, nullptr);
        glfwSetKeyCallback(window.handle, callback);
        callback(window.handle, GLFW_KEY_SPACE, 0, GLFW_PRESS, 0);
    }

    Pixels OpenGlContext::pixels() const
    {
        const auto size = static_cast<std::size_t>(resources->width) *
            static_cast<std::size_t>(resources->height) * 4;
        Pixels result(size);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glReadPixels(0, 0, resources->width, resources->height, GL_RGBA,
                     GL_UNSIGNED_BYTE, result.data());
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        return result;
    }

    SourcePixel OpenGlContext::source_pixel(int x, int y) const
    {
        SourcePixel result{};
        glBindFramebuffer(GL_READ_FRAMEBUFFER, resources->framebuffer);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, result.data());
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        return result;
    }

    DisplayPixel display_pixel(int x, int y)
    {
        DisplayPixel result{};
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, result.data());
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        return result;
    }
} // namespace msc::render::test
