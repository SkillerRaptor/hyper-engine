/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class CommandList;
    class GraphicsDevice;
    class ImGuiManager;
    class Surface;
    class TextureView;
    class SdlEvent;

    class ImGuiPass
    {
    public:
        ImGuiPass(const std::shared_ptr<Surface> &surface);
        ~ImGuiPass();

        void render(const std::shared_ptr<CommandList> &command_list, const std::shared_ptr<TextureView> &swapchain_texture_view) const;

    private:
        static void on_sdl_event(const SdlEvent &event);

    private:
        std::shared_ptr<ImGuiManager> m_imgui_manager;
    };
} // namespace hyper_engine