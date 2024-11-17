/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace he
{
    namespace event
    {
        class EventBus;
    }

    namespace platform
    {
        class SdlEvent;
        class Window;
    } // namespace platform

    namespace rhi
    {
        class CommandList;
        class GraphicsDevice;
        class ImGuiManager;
        class Surface;
        class TextureView;
    } // namespace rhi

    namespace render
    {
        class ImGuiPass
        {
        public:
            ImGuiPass(
                he::event::EventBus &event_bus,
                const he::platform::Window &window,
                const std::shared_ptr<he::rhi::GraphicsDevice> &graphics_device,
                const std::shared_ptr<he::rhi::Surface> &surface);
            ~ImGuiPass();

            void render(
                const std::shared_ptr<he::rhi::CommandList> &command_list,
                const std::shared_ptr<he::rhi::TextureView> &swapchain_texture_view) const;

        private:
            static void on_sdl_event(const he::platform::SdlEvent &event);

        private:
            std::shared_ptr<he::rhi::ImGuiManager> m_imgui_manager;
        };
    } // namespace render
} // namespace he