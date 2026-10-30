#include "opengl_test_context.hh"

#include "appearance_trial_private.hh"

#include <glad/gl.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <string>

#include <mystarceiling/core/appearance/scintillation.hh>
#include <mystarceiling/render/appearance_trial.hh>

using msc::render::TrialStar;
using msc::render::test::OpenGlContext;
namespace trial = msc::render::trial;
using msc::render::TrialSettings;

namespace
{
    using msc::core::appearance::sample_twinkle;
    using msc::core::appearance::scintillation_frame;
    using VertexLights = std::array<std::array<float, 3>, 4>;

    void enable_feedback(trial::Resources& resources,
                         const TrialSettings& settings)
    {
        const char* varying = "star_light";
        glTransformFeedbackVaryings(resources.stars, 1, &varying,
                                    GL_INTERLEAVED_ATTRIBS);
        // The production program retains its attached (delete-marked)
        // shaders. Relink that exact program; do not copy the GLSL here.
        glLinkProgram(resources.stars);
        GLint linked{}, length{};
        glGetProgramiv(resources.stars, GL_LINK_STATUS, &linked);
        glGetProgramiv(resources.stars, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<std::size_t>(std::max(length, 1)), '\0');
        glGetProgramInfoLog(resources.stars, length, nullptr, log.data());
        ASSERT_EQ(linked, GL_TRUE) << log;
        // Relinking clears uniforms; render() restores all remaining ones.
        trial::initialize_rays(resources.stars, settings);
        ASSERT_EQ(glGetError(), GL_NO_ERROR);
    }

    struct Feedback
    {
        GLuint buffer{}, query{};

        Feedback()
        {
            glGenBuffers(1, &buffer);
            glGenQueries(1, &query);
            glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer);
            glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(VertexLights),
                         nullptr, GL_STREAM_READ);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);
        }
        Feedback(const Feedback&) = delete;
        Feedback& operator=(const Feedback&) = delete;
        ~Feedback()
        {
            glDeleteQueries(1, &query);
            glDeleteBuffers(1, &buffer);
        }
    };

    VertexLights capture_light(const trial::Resources& resources)
    {
        Feedback feedback;
        glUseProgram(resources.stars);
        glBindVertexArray(resources.vao);
        glEnable(GL_RASTERIZER_DISCARD);
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, feedback.query);
        // Point topology records each of the four submitted quad vertices
        // once, without the triangle strip's repeated triangle vertices.
        glBeginTransformFeedback(GL_POINTS);
        glDrawArraysInstanced(GL_POINTS, 0, 4, 1);
        glEndTransformFeedback();
        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        glDisable(GL_RASTERIZER_DISCARD);
        GLuint written{};
        glGetQueryObjectuiv(feedback.query, GL_QUERY_RESULT, &written);
        EXPECT_EQ(written, 4U);
        VertexLights result{};
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(result),
                           result.data());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
        return result;
    }

    void expect_reference(const VertexLights& light, const TrialStar& star,
                          const trial::State& state)
    {
        const auto frame =
            scintillation_frame(state.clock.seconds(), state.clock.exposure());
        const auto gain = sample_twinkle(star.animation_key, frame,
                                         star.enu.z(), state.settings.twinkle);
        const std::array channels{star.colour.red, star.colour.green,
                                  star.colour.blue};
        for (std::size_t vertex = 0; vertex < light.size(); ++vertex)
            for (std::size_t channel = 0; channel < gain.size(); ++channel)
            {
                SCOPED_TRACE(testing::Message()
                             << "vertex=" << vertex << " channel=" << channel);
                const auto base = channels[channel] / 255.0 *
                    state.settings.brightness *
                    std::pow(10.0, -0.4 * star.magnitude);
                // Absolute gain error: float phase reduction/trig and a
                // 24-term sum, amplified by response slope <=10 and
                // strength 0.12. 2e-5 is ~170 float epsilons; scale by
                // source light, not by the (possibly dark) modulated
                // channel value.
                EXPECT_NEAR(light[vertex][channel], base * gain[channel],
                            2e-5 * base);
            }
    }

    void expect_negative_bands(double seconds, double exposure)
    {
        const auto frame = scintillation_frame(seconds, exposure);
        EXPECT_TRUE(std::ranges::any_of(frame, [](const auto& band)
                                        { return band[1] < 0; }));
    }

    void exercise_reference_samples(OpenGlContext& context,
                                    const TrialStar& star)
    {
        for (const double seconds : {0.125, 17.125, 4096.5})
            for (const double interval : {1.0 / 60, 1.0 / 30, 0.125})
                for (const double brightness : {3.25, 14.0})
                {
                    SCOPED_TRACE(testing::Message()
                                 << "t=" << seconds << " dt=" << interval
                                 << " brightness=" << brightness);
                    context.state.settings.brightness = brightness;
                    context.state.clock = {};
                    context.state.clock.advance(seconds - interval);
                    context.state.clock.advance(interval);
                    expect_negative_bands(context.state.clock.seconds(),
                                          interval);
                    ASSERT_NO_FATAL_FAILURE(context.draw());
                    expect_reference(capture_light(*context.resources), star,
                                     context.state);
                }
    }
} // namespace

TEST(OpenGlRegression, ProductionVertexLightMatchesCpuForEveryQuadVertex)
{
    OpenGlContext context;
    const auto available = context.open();
    if (!available)
        GTEST_SKIP() << available.error();
    const std::array stars{TrialStar{{0, 0, 1}, -1.25, {255, 128, 32}, {0, 0}},
                           TrialStar{{0, 0.8660254037844386, 0.5},
                                     1.125,
                                     {17, 255, 93},
                                     {1, 0x80000000U}},
                           TrialStar{{0.9921567416492215, 0, 0.125},
                                     5.5,
                                     {0, 1, 255},
                                     {0xffffffffU, 0x12345678U}}};
    for (const auto& star : stars)
    {
        SCOPED_TRACE(testing::Message() << "key=" << star.animation_key.low
                                        << ',' << star.animation_key.high
                                        << " elevation=" << star.enu.z());
        ASSERT_NO_FATAL_FAILURE(context.initialize_scene(std::array{star}));
        ASSERT_EQ(context.resources->count, 1);
        ASSERT_NO_FATAL_FAILURE(
            enable_feedback(*context.resources, context.state.settings));
        ASSERT_NO_FATAL_FAILURE(exercise_reference_samples(context, star));
    }
}

TEST(OpenGlRegression, StartupClipsExposureAndMatchesCpuLight)
{
    OpenGlContext context;
    const auto available = context.open();
    if (!available)
        GTEST_SKIP() << available.error();
    RecordProperty("renderer",
                   reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    RecordProperty("version",
                   reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    const TrialStar star{{0, 0.6, 0.8}, 2, {255, 128, 32}, {17, 0xabcdef12U}};
    ASSERT_NO_FATAL_FAILURE(context.initialize_scene(std::array{star}));
    ASSERT_NO_FATAL_FAILURE(
        enable_feedback(*context.resources, context.state.settings));
    ASSERT_NO_FATAL_FAILURE(context.draw());
    expect_reference(capture_light(*context.resources), star, context.state);
    context.state.clock.advance(1.0 / 240);
    ASSERT_NO_FATAL_FAILURE(context.draw());
    expect_reference(capture_light(*context.resources), star, context.state);
}
