/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_engine/engine_loop.hpp"

#include <chrono>
#include <exception>
#include <ranges>
#include <string>
#include <vector>

#include <argparse/argparse.hpp>

#include <hyper_core/assertion.hpp>
#include <hyper_core/job_system.hpp>
#include <hyper_core/logger.hpp>
#include <hyper_core/prerequisites.hpp>
#include <hyper_event/event_bus.hpp>
#include <hyper_platform/input.hpp>
#include <hyper_platform/window_events.hpp>
#include <hyper_platform/window.hpp>
#include <hyper_render/renderer.hpp>
#include <hyper_rhi/graphics_device.hpp>

#include "hyper_engine/editor_engine.hpp"
#include "hyper_engine/game_engine.hpp"

namespace hyper_engine
{
    EngineLoop::EngineLoop()
    {
    }

    EngineLoop::~EngineLoop()
    {
        delete Renderer::get();
        delete GraphicsDevice::get();
        delete Window::get();
        delete Input::get();
        delete EventBus::get();
        delete JobSystem::get();
        delete Logger::get();
    }

    bool EngineLoop::pre_initialize(const int32_t argc, const char **argv)
    {
        const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

        Logger::get() = new Logger();
        JobSystem::get() = new JobSystem();

        const std::vector<std::string> arguments(argv, argv + argc);

        argparse::ArgumentParser program("HyperEngine");

        std::string log_level = "info";
        program.add_argument("--level")
            .default_value("info")
            .choices("info", "warning", "error", "critical", "debug", "trace")
            .store_into(log_level);

        program.add_argument("--editor").default_value(false).implicit_value(false).store_into(m_editor_enabled);

        std::string renderer;
        program.add_argument("--renderer").default_value("vulkan").choices("d3d12", "vulkan").store_into(renderer);

        bool debug_validation_enabled = false;
        program.add_argument("--debug-validation").default_value(false).implicit_value(true).store_into(debug_validation_enabled);

        bool debug_label_enabled = false;
        program.add_argument("--debug-label").default_value(false).implicit_value(true).store_into(debug_label_enabled);

        bool debug_marker_enabled = false;
        program.add_argument("--debug-marker").default_value(false).implicit_value(true).store_into(debug_marker_enabled);

        try
        {
            program.parse_args(arguments);
        }
        catch (const std::exception &error)
        {
            HE_CRITICAL("Failed to parse arguments: {}", error.what());
            return false;
        }

        const spdlog::level::level_enum level = [log_level]()
        {
            if (log_level == "trace")
            {
                return spdlog::level::trace;
            }

            if (log_level == "debug")
            {
                return spdlog::level::debug;
            }

            if (log_level == "info")
            {
                return spdlog::level::info;
            }

            if (log_level == "warning")
            {
                return spdlog::level::warn;
            }

            if (log_level == "error")
            {
                return spdlog::level::err;
            }

            if (log_level == "critical")
            {
                return spdlog::level::critical;
            }

            HE_UNREACHABLE();
        }();

        Logger::get()->set_level(level);

        EventBus::get() = new EventBus();
        Input::get() = new Input();
        Window::get() = new Window({
            .title = "HyperEngine",
            .width = 1280,
            .height = 720,
        });

        const GraphicsApi graphics_api = [renderer]()
        {
            if (renderer == "d3d12")
            {
                return GraphicsApi::D3D12;
            }

            if (renderer == "vulkan")
            {
                return GraphicsApi::Vulkan;
            }

            HE_UNREACHABLE();
        }();

        GraphicsDevice::get() = GraphicsDevice::create({
            .graphics_api = graphics_api,
            .debug_validation = debug_validation_enabled,
            .debug_label = debug_label_enabled,
            .debug_marker = debug_marker_enabled,
        });

        Renderer::get() = new Renderer();

        EventBus::get()->subscribe<WindowCloseEvent>(HE_BIND_FUNCTION(EngineLoop::on_close));

        const std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        HE_INFO("Engine pre-initialized in {:.2}s", elapsed_seconds.count());

        return true;
    }

    bool EngineLoop::initialize()
    {
        const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

        if (m_editor_enabled)
        {
            m_engine = make_own<EditorEngine>();
        }
        else
        {
            m_engine = make_own<GameEngine>();
        }

        if (!m_engine->initialize())
        {
            HE_CRITICAL("Failed to initialize the engine!");
            return false;
        }

        const std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsed_seconds = end_time - start_time;
        HE_INFO("Engine initialized in {:.2}s", elapsed_seconds.count());

        return true;
    }

    void EngineLoop::run()
    {
        float total_time = 0.0;
        constexpr float delta_time = 1.0f / 60.0f;

        float accumulator = 0.0;
        std::chrono::time_point current_time = std::chrono::steady_clock::now();
        while (!m_exit_requested)
        {
            // Update frame time
            const std::chrono::time_point new_time = std::chrono::steady_clock::now();
            const float frame_time = std::chrono::duration<float>(new_time - current_time).count();
            current_time = new_time;

            accumulator += frame_time;

            // Handle Events
            Window::get()->process_events();
            while (Window::get()->width() == 0 || Window::get()->height() == 0)
            {
                Window::wait_events();
            }

            while (accumulator >= delta_time)
            {
                // Fixed Update
                m_engine->fixed_update(delta_time, total_time);

                accumulator -= delta_time;
                total_time += delta_time;
            }

            // Update
            m_engine->update(delta_time, total_time);

            // Render
            const Camera &camera = m_engine->camera();
            Renderer::get()->begin_frame({
                .position = camera.position(),
                .view = camera.view_matrix(),
                .projection = camera.projection_matrix(),
                .near_plane = camera.near_plane(),
                .far_plane = camera.far_plane(),
            });

            Renderer::get()->render_scene(m_engine->scene());
            m_engine->render();

            Renderer::get()->end_frame();
            Renderer::get()->present();
        }
    }

    void EngineLoop::on_close(const WindowCloseEvent &)
    {
        m_exit_requested = true;
    }
} // namespace hyper_engine