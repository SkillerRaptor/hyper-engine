/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_queue.hpp"

#include "hyper_rhi/vulkan/vulkan_command_list.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanQueue::VulkanQueue(VulkanGraphicsDevice &graphics_device, uint32_t queue_family, const VkQueue queue)
        : m_graphics_device(graphics_device)
        , m_queue_family(queue_family)
        , m_queue(queue)
        , m_submit_semaphore(VK_NULL_HANDLE)
    {
        HE_TRACE("Retrieved queue from the family #{}", m_queue_family);

        VkSemaphoreTypeCreateInfo submit_semaphore_type_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
            .pNext = nullptr,
            .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
            .initialValue = 0,
        };

        const VkSemaphoreCreateInfo submit_semaphore_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = &submit_semaphore_type_create_info,
            .flags = 0,
        };

        HE_VK_CHECK(vkCreateSemaphore(m_graphics_device.device(), &submit_semaphore_create_info, nullptr, &m_submit_semaphore));
        HE_ASSERT(m_submit_semaphore != VK_NULL_HANDLE);

        HE_TRACE("Created Submit Semaphore for queue family #{}", m_queue_family);
    }

    VulkanQueue::~VulkanQueue()
    {
        vkDestroySemaphore(m_graphics_device.device(), m_submit_semaphore, nullptr);
    }

    void VulkanQueue::submit(const CommandListHandle command_list_handle)
    {
        if (!m_buffer_writes.empty())
        {
            m_graphics_device.upload_manager().upload(std::exchange(m_buffer_writes, {}));
        }

        if (command_list_handle)
        {
            const std::shared_ptr<VulkanCommandList> command_list = std::dynamic_pointer_cast<VulkanCommandList>(command_list_handle);

            const VkSemaphoreSubmitInfo semaphore_submit_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                .pNext = nullptr,
                .semaphore = m_graphics_device.current_frame().present_semaphore,
                .value = 0,
                .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .deviceIndex = 0,
            };

            const VkCommandBufferSubmitInfo command_buffer_submit_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
                .pNext = nullptr,
                .commandBuffer = command_list->command_buffer(),
                .deviceMask = 0,
            };

            const VkSemaphoreSubmitInfo submit_semaphore_submit_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                .pNext = nullptr,
                .semaphore = m_submit_semaphore,
                .value = m_graphics_device.current_frame_index(),
                .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .deviceIndex = 0,
            };

            const VkSemaphoreSubmitInfo render_semaphore_submit_info = {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                .pNext = nullptr,
                .semaphore = m_graphics_device.current_frame().render_semaphore,
                .value = 0,
                .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .deviceIndex = 0,
            };

            const std::array<VkSemaphoreSubmitInfo, 2> signal_semaphore_submit_infos = {
                submit_semaphore_submit_info,
                render_semaphore_submit_info,
            };

            const VkSubmitInfo2 submit_info = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
                .pNext = nullptr,
                .flags = 0,
                .waitSemaphoreInfoCount = 1,
                .pWaitSemaphoreInfos = &semaphore_submit_info,
                .commandBufferInfoCount = 1,
                .pCommandBufferInfos = &command_buffer_submit_info,
                .signalSemaphoreInfoCount = static_cast<uint32_t>(signal_semaphore_submit_infos.size()),
                .pSignalSemaphoreInfos = signal_semaphore_submit_infos.data(),
            };

            HE_VK_CHECK(vkQueueSubmit2(m_queue, 1, &submit_info, VK_NULL_HANDLE));
        }
    }

    uint32_t VulkanQueue::queue_family() const
    {
        return m_queue_family;
    }

    VkQueue VulkanQueue::queue() const
    {
        return m_queue;
    }

    VkSemaphore VulkanQueue::submit_semaphore() const
    {
        return m_submit_semaphore;
    }
} // namespace hyper_rhi