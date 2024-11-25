/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/imgui_manager.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanImGuiManager final : public IImGuiManager
    {
    public:
        explicit VulkanImGuiManager(VulkanGraphicsDevice &graphics_device);

        void initialize(const std::shared_ptr<ISurface> &surface) override;
        void destroy() override;

        void new_frame() override;
        void render(const std::shared_ptr<ICommandList> &command_list) override;

        void on_resize(uint32_t width, uint32_t height) override;

    private:
        VulkanGraphicsDevice &m_graphics_device;
    };
} // namespace hyper_engine