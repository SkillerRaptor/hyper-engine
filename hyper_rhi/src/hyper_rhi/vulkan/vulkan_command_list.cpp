/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_command_list.hpp"

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanCommandList::VulkanCommandList(VulkanGraphicsDevice &graphics_device)
        : m_graphics_device(graphics_device)
    {
    }

    void VulkanCommandList::begin() const
    {
        const VkCommandBuffer &command_buffer = m_graphics_device.current_frame().command_buffer;

        vkResetCommandBuffer(command_buffer, 0);

        constexpr VkCommandBufferBeginInfo command_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr,
        };
        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
    }

    void VulkanCommandList::end() const
    {
        const VkCommandBuffer &command_buffer = m_graphics_device.current_frame().command_buffer;

        vkEndCommandBuffer(command_buffer);
    }

    void VulkanCommandList::write_buffer(const BufferHandle buffer, const void *data, const size_t size)
    {
        HE_UNUSED(buffer);
        HE_UNUSED(data);
        HE_UNUSED(size);

        // TODO: Add Queue class
        HE_UNREACHABLE();
    }
} // namespace hyper_rhi