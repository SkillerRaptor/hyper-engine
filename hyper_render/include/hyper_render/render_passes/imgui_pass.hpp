/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class EventBus;
    class ICommandList;
    class IGraphicsDevice;
    class IImGuiManager;
    class ISurface;
    class ITextureView;
    class SdlEvent;
    class Window;

    class ImGuiPass
    {
    public:
        ImGuiPass(
            EventBus &event_bus,
            const Window &window,
            const std::shared_ptr<IGraphicsDevice> &graphics_device,
            const std::shared_ptr<ISurface> &surface);
        ~ImGuiPass();

        void render(const std::shared_ptr<ICommandList> &command_list, const std::shared_ptr<ITextureView> &swapchain_texture_view) const;

    private:
        static void on_sdl_event(const SdlEvent &event);

    private:
        std::shared_ptr<IImGuiManager> m_imgui_manager;
    };
} // namespace hyper_engine