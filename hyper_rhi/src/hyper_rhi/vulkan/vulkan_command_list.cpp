/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_command_list.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_render_pass.hpp"

namespace hyper_rhi
{
    VulkanCommandList::VulkanCommandList(VulkanGraphicsDevice &graphics_device)
        : m_graphics_device(graphics_device)
        , m_command_buffer(VK_NULL_HANDLE)
    {
    }

    void VulkanCommandList::begin()
    {
        m_command_buffer = m_graphics_device.current_frame().command_buffer;

        HE_VK_CHECK(vkResetCommandBuffer(m_command_buffer, 0));

        constexpr VkCommandBufferBeginInfo command_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr,
        };

        HE_VK_CHECK(vkBeginCommandBuffer(m_command_buffer, &command_buffer_begin_info));
    }

    void VulkanCommandList::end()
    {
        HE_VK_CHECK(vkEndCommandBuffer(m_command_buffer));
    }

    RenderPassHandle VulkanCommandList::begin_render_pass(const RenderPassDescriptor &descriptor) const
    {
        return std::make_shared<VulkanRenderPass>(m_graphics_device, m_command_buffer, descriptor);
    }

    VkCommandBuffer VulkanCommandList::command_buffer() const
    {
        return m_command_buffer;
    }
} // namespace hyper_rhi