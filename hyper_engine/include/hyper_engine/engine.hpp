/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <chrono>
#include <memory>

#include <hyper_event/event_bus.hpp>
#include <hyper_platform/input.hpp>
#include <hyper_platform/window_events.hpp>
#include <hyper_platform/window.hpp>
#include <hyper_rhi/graphics_device.hpp>
#include <hyper_render/renderer.hpp>

namespace hyper_engine
{
    struct EngineDescriptor
    {
        uint32_t width;
        uint32_t height;
        hyper_rhi::GraphicsApi graphics_api;
        bool debug_validation;
        bool debug_label;
        bool debug_marker;
    };

    class Engine
    {
    public:
        explicit Engine(const EngineDescriptor &descriptor);

        void run();

    private:
        void on_close(const hyper_platform::WindowCloseEvent &event);
        void on_resize(const hyper_platform::WindowResizeEvent &event) const;

    private:
        std::chrono::steady_clock::time_point m_start_time;

        bool m_running;
        hyper_event::EventBus m_event_bus;
        hyper_platform::Window m_window;
        std::shared_ptr<hyper_rhi::GraphicsDevice> m_graphics_device;
        std::shared_ptr<hyper_rhi::Surface> m_surface;
        hyper_render::Renderer m_renderer;
    };
} // namespace hyper_engine
