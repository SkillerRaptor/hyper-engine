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

        void begin() const override;
        void end() const override;

        void write_buffer(BufferHandle buffer, const void *data, size_t size) override;

    private:
        VulkanGraphicsDevice &m_graphics_device;
    };
} // namespace hyper_rhi