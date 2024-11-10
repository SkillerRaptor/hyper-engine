/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_platform/sdl_event.hpp"

#include <hyper_rhi/graphics_device.hpp>
#include <hyper_rhi/imgui_manager.hpp>

namespace hyper_render
{
    class ImGuiPass
    {
    public:
        ImGuiPass(
            hyper_event::EventBus &event_bus,
            const hyper_platform::Window &window,
            const std::shared_ptr<hyper_rhi::GraphicsDevice> &graphics_device,
            const std::shared_ptr<hyper_rhi::Surface> &surface);
        ~ImGuiPass();

        void render(
            const std::shared_ptr<hyper_rhi::CommandList> &command_list,
            const std::shared_ptr<hyper_rhi::TextureView> &swapchain_texture_view) const;

    private:
        static void on_sdl_event(const hyper_platform::SdlEvent &event);

    private:
        std::shared_ptr<hyper_rhi::ImGuiManager> m_imgui_manager;
    };
} // namespace hyper_render