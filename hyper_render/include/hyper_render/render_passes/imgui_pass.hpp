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
        class ICommandList;
        class IGraphicsDevice;
        class IImGuiManager;
        class ISurface;
        class ITextureView;
    } // namespace rhi

    namespace render
    {
        class ImGuiPass
        {
        public:
            ImGuiPass(
                he::event::EventBus &event_bus,
                const he::platform::Window &window,
                const std::shared_ptr<he::rhi::IGraphicsDevice> &graphics_device,
                const std::shared_ptr<he::rhi::ISurface> &surface);
            ~ImGuiPass();

            void render(
                const std::shared_ptr<he::rhi::ICommandList> &command_list,
                const std::shared_ptr<he::rhi::ITextureView> &swapchain_texture_view) const;

        private:
            static void on_sdl_event(const he::platform::SdlEvent &event);

        private:
            std::shared_ptr<he::rhi::IImGuiManager> m_imgui_manager;
        };
    } // namespace render
} // namespace he