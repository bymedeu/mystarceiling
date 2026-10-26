#include "appearance_trial_private.hh"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace msc::render::trial
{
    namespace
    {
        void adjust_profile(TrialSettings& s, int key)
        {
            switch (key)
            {
            case GLFW_KEY_H:
                s.halo_fraction = std::max(0.0, s.halo_fraction - .02);
                break;
            case GLFW_KEY_J:
                s.halo_fraction =
                    std::min(1 - s.ray_fraction, s.halo_fraction + .02);
                break;
            case GLFW_KEY_F:
                s.ray_fraction = std::max(0.0, s.ray_fraction - .02);
                break;
            case GLFW_KEY_R:
                s.ray_fraction =
                    std::min(1 - s.halo_fraction, s.ray_fraction + .02);
                break;
            case GLFW_KEY_G:
                s.ray_length = std::max(.5, s.ray_length - .25);
                break;
            case GLFW_KEY_T:
                s.ray_length = std::min(32.0, s.ray_length + .25);
                break;
            case GLFW_KEY_K:
                s.limiting_magnitude =
                    std::max(-30.0, s.limiting_magnitude - .25);
                break;
            case GLFW_KEY_L:
                s.limiting_magnitude =
                    std::min(30.0, s.limiting_magnitude + .25);
                break;
            default:
                break;
            }
        }

        void adjust_animation(TrialSettings& s, int key)
        {
            switch (key)
            {
            case GLFW_KEY_LEFT_BRACKET:
                s.animation_speed = std::max(.01, s.animation_speed / 1.1);
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                s.animation_speed = std::min(10.0, s.animation_speed * 1.1);
                break;
            case GLFW_KEY_DOWN:
                s.twinkle.strength = std::max(0.0, s.twinkle.strength - .05);
                break;
            case GLFW_KEY_UP:
                s.twinkle.strength = std::min(.95, s.twinkle.strength + .05);
                break;
            case GLFW_KEY_LEFT:
                s.twinkle.colour_variation =
                    std::max(0.0, s.twinkle.colour_variation - .05);
                break;
            case GLFW_KEY_RIGHT:
                s.twinkle.colour_variation =
                    std::min(.95, s.twinkle.colour_variation + .05);
                break;
            case GLFW_KEY_MINUS:
                s.brightness = std::max(.01, s.brightness / 1.15);
                break;
            case GLFW_KEY_EQUAL:
                s.brightness = std::min(1000.0, s.brightness * 1.15);
                break;
            default:
                break;
            }
        }

        void key_event(GLFWwindow* window, int key, int, int action, int)
        {
            if (action != GLFW_PRESS && action != GLFW_REPEAT)
                return;
            auto& state =
                *static_cast<State*>(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            {
                state.settings.picture_mode = !state.settings.picture_mode;
                state.clock.set_picture_mode(state.settings.picture_mode);
            }
            adjust_animation(state.settings, key);
            adjust_profile(state.settings, key);
            update_title(window, state);
        }
    } // namespace

    void install_controls(GLFWwindow* window, State& state)
    {
        glfwSetWindowUserPointer(window, &state);
        glfwSetKeyCallback(window, key_event);
        state.clock.set_picture_mode(state.settings.picture_mode);
        update_title(window, state);
    }

    void update_title(GLFWwindow* window, const State& state)
    {
        const auto& s = state.settings;
        std::ostringstream title;
        title << std::fixed << std::setprecision(2) << "MSC appearance trial | "
              << (s.picture_mode ? "picture" : "animated") << " | light "
              << s.brightness << " speed " << s.animation_speed << " twinkle "
              << s.twinkle.strength << " colour " << s.twinkle.colour_variation
              << " halo " << s.halo_fraction << " rays " << s.ray_fraction
              << " length " << s.ray_length << " mag " << s.limiting_magnitude
              << " | Space freeze, +/- light, [/] speed, arrows "
                 "twinkle/colour, Esc "
                 "close";
        glfwSetWindowTitle(window, title.str().c_str());
    }
} // namespace msc::render::trial
