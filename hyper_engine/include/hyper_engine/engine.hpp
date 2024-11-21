/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <chrono>
#include <memory>

#include <hyper_event/event_bus.hpp>
#include <hyper_platform/window.hpp>
#include <hyper_render/renderer.hpp>
#include <hyper_rhi/graphics_device.hpp>

namespace hyper_engine
{
    class WindowCloseEvent;
    class WindowResizeEvent;
    class IGraphicsDevice;
    class ISurface;

    struct EngineDescriptor
    {
        uint32_t width;
        uint32_t height;
        GraphicsApi graphics_api;
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
        void on_close(const WindowCloseEvent &event);
        void on_resize(const WindowResizeEvent &event) const;

    private:
        std::chrono::steady_clock::time_point m_start_time;

        bool m_running;
        EventBus m_event_bus;
        Window m_window;
        std::shared_ptr<IGraphicsDevice> m_graphics_device;
        std::shared_ptr<ISurface> m_surface;
        Renderer m_renderer;
    };
} // namespace hyper_engine
