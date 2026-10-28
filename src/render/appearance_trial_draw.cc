#include "appearance_trial_private.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>

namespace msc::render::trial
{
    namespace
    {
        void uniform(GLuint program, const char* name, double value)
        {
            glUniform1f(glGetUniformLocation(program, name),
                        static_cast<float>(value));
        }

        void set_profile(GLuint program, const TrialSettings& s)
        {
            uniform(program, "brightness", s.brightness);
            uniform(program, "limiting_magnitude", s.limiting_magnitude);
            uniform(program, "core_width", s.core_width);
            uniform(program, "halo_width", s.halo_width);
            uniform(program, "halo_fraction", s.halo_fraction);
            uniform(program, "ray_fraction", s.ray_fraction);
            uniform(program, "ray_width", s.ray_width);
            uniform(program, "ray_length", s.ray_length);
            const auto sigma = std::max(
                {s.core_width, s.halo_width, s.ray_length, s.ray_width});
            const auto radius =
                s.support_sigma * std::sqrt(sigma * sigma + 1.0 / 12);
            uniform(program, "support_radius", radius);
        }

        void animation(GLuint program, const State& state)
        {
            const auto& settings = state.settings.twinkle;
            const auto frame = core::appearance::scintillation_frame(
                state.clock.seconds(), state.clock.exposure());
            static_assert(sizeof(frame) == frame.size() * 3 * sizeof(float));
            glUniform3fv(glGetUniformLocation(program, "bands"),
                         static_cast<GLsizei>(frame.size()),
                         frame.front().data());
            uniform(program, "strength", settings.strength);
            uniform(program, "zenith_fraction", settings.zenith_fraction);
            uniform(program, "colour_delay", settings.colour_delay);
        }

        void draw_stars(Resources& resources, const State& state)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, resources.framebuffer);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glUseProgram(resources.stars);
            glBindVertexArray(resources.vao);
            glUniform2f(
                glGetUniformLocation(resources.stars, "framebuffer_size"),
                static_cast<float>(resources.width),
                static_cast<float>(resources.height));
            set_profile(resources.stars, state.settings);
            animation(resources.stars, state);
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, resources.count);
        }

        void display(const Resources& resources)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_BLEND);
            glUseProgram(resources.display);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, resources.texture);
            glUniform1i(glGetUniformLocation(resources.display, "accumulated"),
                        0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    } // namespace

    void initialize_rays(GLuint program, const TrialSettings& settings)
    {
        std::array<std::array<float, 4>, 24> rays{};
        const core::appearance::AnimationKey key{0x734f9aU, 0};
        double total{};
        for (int i = 0; i < settings.ray_count; ++i)
        {
            const auto index = static_cast<std::size_t>(i);
            const auto n = core::appearance::hash_noise(
                key, static_cast<std::uint32_t>(i));
            const auto a = 2 * std::numbers::pi *
                (i + .3 * (static_cast<double>(n & 1023U) / 1023 - .5)) /
                settings.ray_count;
            const auto length =
                .4F + .6F * static_cast<float>((n >> 10U) & 1023U) / 1023;
            const auto weight =
                .25F + .75F * static_cast<float>((n >> 20U) & 1023U) / 1023;
            rays[index] = {static_cast<float>(std::cos(a)),
                           static_cast<float>(std::sin(a)), length, weight};
            total += weight;
        }
        for (auto& ray : rays)
            ray[3] /= static_cast<float>(total);
        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "ray_count"),
                    settings.ray_count);
        glUniform4fv(glGetUniformLocation(program, "rays"), 24,
                     rays.front().data());
    }

    std::expected<void, std::string> render(Resources& resources,
                                            const State& state)
    {
        glViewport(0, 0, resources.width, resources.height);
        draw_stars(resources, state);
        display(resources);
        if (glGetError() != GL_NO_ERROR)
            return std::unexpected{"OpenGL appearance trial draw failed"};
        return {};
    }
} // namespace msc::render::trial
