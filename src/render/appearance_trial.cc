#include <GLFW/glfw3.h>
#include <expected>
#include <mystarceiling/render/appearance_trial.hh>
#include <span>
#include <string>

#include "appearance_trial_private.hh"

#include <chrono>
#include <exception>
#include <thread>
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
                state.clock.advance(current - previous);
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
                const auto spare = 1.0 / 30 - (glfwGetTime() - current);
                if (spare > 0)
                    std::this_thread::sleep_for(
                        std::chrono::duration<double>(spare));
            }
            return {};
        }
    } // namespace

    std::expected<void, std::string>
    show_appearance_trial(std::span<const TrialStar> stars,
                          TrialSettings settings)
    {
        if (!valid_trial_settings(settings))
            return std::unexpected{"Invalid MSC viewer settings"};
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
            return std::unexpected{"MSC viewer failed: " +
                                   std::string(error.what())};
        }
    }
} // namespace msc::render
