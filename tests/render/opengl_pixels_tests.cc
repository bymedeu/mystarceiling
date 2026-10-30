#include "opengl_test_context.hh"

#include "appearance_trial_private.hh"

#include <glad/gl.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <limits>

#include <mystarceiling/core/astronomy/linear_rgb8.hh>
#include <mystarceiling/render/appearance_trial.hh>

using msc::render::TrialStar;
using msc::render::test::OpenGlContext;
namespace trial = msc::render::trial;
using msc::render::test::display_pixel;
using msc::render::test::DisplayPixel;
using msc::render::test::Pixels;
using msc::render::test::SourcePixel;

namespace
{
    using msc::core::astronomy::LinearRgb8;

    double encode_srgb(double linear)
    {
        return linear <= 0.0031308
            ? 12.92 * linear
            : 1.055 * std::pow(linear, 1.0 / 2.4) - 0.055;
    }

    void expect_display_transfer(const SourcePixel& source,
                                 const DisplayPixel& display)
    {
        // Documented viewer transfer: L/(1+max(R,G,B)), then the standard
        // piecewise sRGB OETF. The source attachment is linear RGBA32F.
        const auto denominator = 1.0 + *std::ranges::max_element(source);
        for (std::size_t channel = 0; channel < source.size(); ++channel)
        {
            SCOPED_TRACE(testing::Message() << "channel=" << channel);
            const auto expected =
                255 * encode_srgb(source[channel] / denominator);
            // At least 8-bit colour: allow one LSB for framebuffer rounding
            // /dither plus one for float transfer and readback rounding.
            // This is a transfer check, not a cross-driver image hash.
            EXPECT_NEAR(display[channel], expected, 2.0);
        }
        EXPECT_EQ(display[3], 255);
    }

    void expect_rgb_proportions(const SourcePixel& source, LinearRgb8 colour)
    {
        ASSERT_GT(source[0], 0.0F);
        const std::array channels{colour.red, colour.green, colour.blue};
        for (std::size_t channel = 0; channel < source.size(); ++channel)
            EXPECT_NEAR(source[channel] / source[0],
                        double(channels[channel]) / colour.red, 2e-6)
                << "linear RGB8 channel=" << channel;
    }

    void expect_eight_bit_colour()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for (const auto channel : {GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
                                   GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
                                   GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE})
        {
            GLint bits{};
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_FRONT_LEFT,
                                                  channel, &bits);
            EXPECT_GE(bits, 8);
        }
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }

    void expect_freeze_and_resume(OpenGlContext& context,
                                  const Pixels& animated)
    {
        ASSERT_NO_FATAL_FAILURE(context.toggle_picture());
        ASSERT_TRUE(context.state.settings.picture_mode);
        const auto frozen_time = context.state.clock.seconds();
        const auto frozen_exposure = context.state.clock.exposure();
        for (const auto elapsed : {1.0 / 30, 10.0, 100.0})
        {
            context.state.clock.advance(elapsed);
            ASSERT_NO_FATAL_FAILURE(context.draw());
            EXPECT_EQ(context.pixels(), animated);
            EXPECT_DOUBLE_EQ(context.state.clock.seconds(), frozen_time);
            EXPECT_DOUBLE_EQ(context.state.clock.exposure(), frozen_exposure);
        }
        ASSERT_NO_FATAL_FAILURE(context.toggle_picture());
        ASSERT_FALSE(context.state.settings.picture_mode);
        context.state.clock.advance(1.0 / 30);
        ASSERT_NO_FATAL_FAILURE(context.draw());
        EXPECT_DOUBLE_EQ(context.state.clock.seconds(), frozen_time + 1.0 / 30);
        EXPECT_NE(context.pixels(), animated);
    }

    void expect_resized_attachment(const trial::Resources& resources)
    {
        EXPECT_EQ(resources.width, 80);
        EXPECT_EQ(resources.height, 72);
        GLint width{}, height{}, format{};
        glBindTexture(GL_TEXTURE_2D, resources.texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT,
                                 &format);
        EXPECT_EQ(width, 80);
        EXPECT_EQ(height, 72);
        EXPECT_EQ(format, GL_RGBA32F);
        std::array<GLint, 4> viewport{};
        glGetIntegerv(GL_VIEWPORT, viewport.data());
        EXPECT_EQ(viewport, (std::array<GLint, 4>{0, 0, 80, 72}));
        EXPECT_EQ(glGetError(), GL_NO_ERROR);
    }
} // namespace

TEST(OpenGlRegression, FloatLightAddsBeforeOneToneMapAndSrgbEncoding)
{
    OpenGlContext context;
    const auto available = context.open();
    if (!available)
        GTEST_SKIP() << available.error();
    context.state.settings.twinkle.strength = 0;
    expect_eight_bit_colour();
    for (const auto colour : {LinearRgb8{255, 255, 255},
                              LinearRgb8{255, 128, 32}, LinearRgb8{255, 96, 1}})
    {
        SCOPED_TRACE(testing::Message()
                     << "RGB8=" << int(colour.red) << ',' << int(colour.green)
                     << ',' << int(colour.blue));
        const TrialStar star{{0, 0, 1}, 1, colour, {81, 9}};
        ASSERT_NO_FATAL_FAILURE(context.initialize_scene(std::array{star}));
        ASSERT_NO_FATAL_FAILURE(context.draw());
        const auto single = context.source_pixel(32, 32);
        ASSERT_NO_FATAL_FAILURE(expect_rgb_proportions(single, colour));
        expect_display_transfer(single, display_pixel(32, 32));
        ASSERT_NO_FATAL_FAILURE(
            context.initialize_scene(std::array{star, star}));
        ASSERT_NO_FATAL_FAILURE(context.draw());
        const auto doubled = context.source_pixel(32, 32);
        for (std::size_t channel = 0; channel < single.size(); ++channel)
            EXPECT_NEAR(doubled[channel], 2.0 * single[channel],
                        4 * std::numeric_limits<float>::epsilon() *
                            single[channel])
                << "additive channel=" << channel;
        expect_display_transfer(doubled, display_pixel(32, 32));
        EXPECT_EQ(glIsEnabled(GL_FRAMEBUFFER_SRGB), GL_FALSE);
    }
}

TEST(OpenGlRegression, ThirtyHzAnimationFreezesAndResumesThroughSpaceControl)
{
    OpenGlContext context;
    const auto available = context.open();
    if (!available)
        GTEST_SKIP() << available.error();
    const std::array stars{
        TrialStar{{0, 0, 1}, 1, {255, 180, 96}, {81, 9}},
        TrialStar{{0.6, 0, 0.8}, 2, {96, 180, 255}, {1, 0x80000000U}},
        TrialStar{{-0.48, 0.36, 0.8}, 3, {255, 255, 255}, {1234, 1}}};
    ASSERT_NO_FATAL_FAILURE(context.initialize_scene(stars));
    ASSERT_NO_FATAL_FAILURE(context.draw());
    const auto initial = context.pixels();
    // Deterministic 30 Hz increments; no sleep or performance measurement.
    for (int frame = 0; frame < 6; ++frame)
    {
        context.state.clock.advance(1.0 / 30);
        ASSERT_NO_FATAL_FAILURE(context.draw());
    }
    const auto animated = context.pixels();
    EXPECT_NE(initial, animated);
    ASSERT_NO_FATAL_FAILURE(expect_freeze_and_resume(context, animated));
}

TEST(OpenGlRegression, ResizeReallocatesFloatAttachmentAndBoundsOutputViewport)
{
    OpenGlContext context;
    const auto available = context.open();
    if (!available)
        GTEST_SKIP() << available.error();
    const TrialStar star{{0, 0, 1}, 1, {255, 128, 32}, {81, 9}};
    ASSERT_NO_FATAL_FAILURE(context.initialize_scene(std::array{star}));
    ASSERT_NO_FATAL_FAILURE(context.draw());
    EXPECT_GT(context.source_pixel(32, 32)[0], 0.0F);
    // NULL/EGL's pbuffer stays 96x80. Exercise production FBO/viewport
    // resizing inside that surface, rather than claiming window-system
    // resize.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    const auto resized = trial::resize(*context.resources, 80, 72);
    ASSERT_TRUE(resized) << resized.error();
    ASSERT_NO_FATAL_FAILURE(context.draw());
    expect_resized_attachment(*context.resources);
    EXPECT_EQ(context.pixels().size(), 80U * 72U * 4U);
    EXPECT_GT(context.source_pixel(40, 36)[0], 0.0F);
    EXPECT_GT(display_pixel(40, 36)[0], 0);
    EXPECT_EQ(display_pixel(79, 71), (DisplayPixel{0, 0, 0, 255}));
    EXPECT_EQ(display_pixel(80, 72), (DisplayPixel{255, 0, 255, 255}));
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
}

TEST(OpenGlRegression, MagnitudeCutoffRetainsOffscreenEdgeLight)
{
    OpenGlContext context;
    const auto available = context.open();
    if (!available)
        GTEST_SKIP() << available.error();
    context.state.settings.twinkle.strength = 0;
    // Centre lies beyond the right edge; part of its light profile remains in
    // view.
    constexpr double ratio = 1.04;
    const auto up = 1 / std::sqrt(1 + ratio * ratio);
    TrialStar star{{ratio * up, 0, up}, 6, {255, 255, 255}, {81, 9}};
    ASSERT_NO_FATAL_FAILURE(context.initialize_scene(std::array{star}));
    ASSERT_NO_FATAL_FAILURE(context.draw());
    EXPECT_GT(context.source_pixel(63, 32)[0], 0.0F);
    star.magnitude = 6.125;
    ASSERT_NO_FATAL_FAILURE(context.initialize_scene(std::array{star}));
    ASSERT_NO_FATAL_FAILURE(context.draw());
    EXPECT_EQ(context.source_pixel(63, 32), (SourcePixel{0, 0, 0}));
}
