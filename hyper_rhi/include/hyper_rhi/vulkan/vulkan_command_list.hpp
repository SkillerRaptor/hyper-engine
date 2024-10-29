/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/command_list.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanCommandList final : public CommandList
    {
    public:
        explicit VulkanCommandList(VulkanGraphicsDevice &graphics_device);

        void begin() override;
        void end() override;

        RenderPassHandle begin_render_pass(const RenderPassDescriptor &descriptor) const override;

        [[nodiscard]] VkCommandBuffer command_buffer() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandBuffer m_command_buffer;
    };
} // namespace hyper_rhi