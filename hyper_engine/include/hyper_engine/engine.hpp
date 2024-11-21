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

namespace he
{
    namespace platform
    {
        class WindowCloseEvent;
        class WindowResizeEvent;
    } // namespace platform

    namespace rhi
    {
        class IGraphicsDevice;
        class ISurface;
    } // namespace rhi

    namespace engine
    {
        struct EngineDescriptor
        {
            uint32_t width;
            uint32_t height;
            he::rhi::GraphicsApi graphics_api;
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
            void on_close(const he::platform::WindowCloseEvent &event);
            void on_resize(const he::platform::WindowResizeEvent &event) const;

        private:
            std::chrono::steady_clock::time_point m_start_time;

            bool m_running;
            he::event::EventBus m_event_bus;
            he::platform::Window m_window;
            std::shared_ptr<he::rhi::IGraphicsDevice> m_graphics_device;
            std::shared_ptr<he::rhi::ISurface> m_surface;
            he::render::Renderer m_renderer;
        };
    } // namespace engine
} // namespace he
