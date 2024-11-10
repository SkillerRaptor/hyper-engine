/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/imgui_manager.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanImGuiManager final : public ImGuiManager
    {
    public:
        explicit VulkanImGuiManager(VulkanGraphicsDevice &graphics_device);

        void initialize(const hyper_platform::Window &window, const std::shared_ptr<Surface> &surface) override;
        void destroy() override;

        void new_frame() override;
        void render(const std::shared_ptr<CommandList> &command_list) override;

        void on_resize(uint32_t width, uint32_t height) override;

    private:
        VulkanGraphicsDevice &m_graphics_device;
    };
} // namespace hyper_rhi