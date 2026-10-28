#include "appearance_trial_private.hh"

namespace msc::render::trial
{
    namespace
    {
        void update_title(GLFWwindow* window, const State& state)
        {
            glfwSetWindowTitle(
                window,
                state.settings.picture_mode
                    ? "MSC viewer | picture | Space resume, Esc close"
                    : "MSC viewer | animated | Space freeze, Esc "
                      "close");
        }
        void key_event(GLFWwindow* window, int key, int, int action, int)
        {
            if (action != GLFW_PRESS)
                return;
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            if (key == GLFW_KEY_SPACE)
            {
                auto& state =
                    *static_cast<State*>(glfwGetWindowUserPointer(window));
                state.settings.picture_mode = !state.settings.picture_mode;
                state.clock.set_picture_mode(state.settings.picture_mode);
                update_title(window, state);
            }
        }
    } // namespace

    void install_controls(GLFWwindow* window, State& state)
    {
        glfwSetWindowUserPointer(window, &state);
        glfwSetKeyCallback(window, key_event);
        state.clock.set_picture_mode(state.settings.picture_mode);
        update_title(window, state);
    }

} // namespace msc::render::trial
