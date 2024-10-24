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

    protected:
        void begin() const override;
        void end() const override;

    private:
        VulkanGraphicsDevice &m_graphics_device;
    };
} // namespace hyper_rhi