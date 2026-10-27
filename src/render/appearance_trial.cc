#include <mystarceiling/render/appearance_trial.hh>

#include "appearance_trial_private.hh"

#include <exception>
#include <utility>

namespace msc::render
{
    namespace
    {
        std::expected<void, std::string> run(trial::Window& window,
                                             trial::Resources& resources,
                                             TrialSettings settings)
        {
            trial::State state{std::move(settings), {}};
            trial::install_controls(window.handle, state);
            auto previous = glfwGetTime();
            while (glfwWindowShouldClose(window.handle) == GLFW_FALSE)
            {
                const auto current = glfwGetTime();
                state.clock.advance(current - previous, 1);
                previous = current;
                int width{}, height{};
                glfwGetFramebufferSize(window.handle, &width, &height);
                if (width == 0 || height == 0)
                {
                    glfwWaitEventsTimeout(.05);
                    continue;
                }
                if (auto result = trial::resize(resources, width, height);
                    !result)
                    return result;
                if (auto result = trial::render(resources, state); !result)
                    return result;
                glfwSwapBuffers(window.handle);
                glfwPollEvents();
            }
            return {};
        }
    } // namespace

    std::expected<void, std::string>
    show_appearance_trial(std::span<const TrialStar> stars,
                          TrialSettings settings)
    {
        if (!valid_trial_settings(settings))
            return std::unexpected{"Invalid appearance trial settings"};
        trial::Window window;
        if (auto result = trial::initialize(window, settings); !result)
            return result;
        trial::Resources resources;
        try
        {
            if (auto result = trial::initialize(resources, stars, settings);
                !result)
                return result;
            return run(window, resources, std::move(settings));
        }
        catch (const std::exception& error)
        {
            return std::unexpected{"Appearance trial failed: " +
                                   std::string(error.what())};
        }
    }
} // namespace msc::render
