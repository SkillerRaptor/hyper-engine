/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_upload_manager.hpp"

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanUploadManager::VulkanUploadManager(VulkanGraphicsDevice &graphics_device)
        : m_graphics_device(graphics_device)
        , m_command_pool(VK_NULL_HANDLE)
        , m_command_buffer(VK_NULL_HANDLE)
        , m_semaphore(VK_NULL_HANDLE)
        , m_value(0)
    {
        const std::shared_ptr<VulkanQueue> queue = std::dynamic_pointer_cast<VulkanQueue>(m_graphics_device.queue());

        const VkCommandPoolCreateInfo command_pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = queue->queue_family(),
        };

        HE_VK_CHECK(vkCreateCommandPool(m_graphics_device.device(), &command_pool_create_info, nullptr, &m_command_pool));
        HE_ASSERT(m_command_pool != VK_NULL_HANDLE);

        m_graphics_device.set_object_name(m_command_pool, ObjectType::CommandPool, "Upload");

        HE_TRACE("Created Upload Command Pool");

        const VkCommandBufferAllocateInfo command_buffer_allocate_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        HE_VK_CHECK(vkAllocateCommandBuffers(m_graphics_device.device(), &command_buffer_allocate_info, &m_command_buffer));
        HE_ASSERT(m_command_buffer != VK_NULL_HANDLE);

        HE_TRACE("Allocated Upload Command Buffer");

        VkSemaphoreTypeCreateInfo semaphore_type_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
            .pNext = nullptr,
            .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
            .initialValue = 0,
        };

        const VkSemaphoreCreateInfo semaphore_create_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = &semaphore_type_create_info,
            .flags = 0,
        };

        HE_VK_CHECK(vkCreateSemaphore(m_graphics_device.device(), &semaphore_create_info, nullptr, &m_semaphore));
        HE_ASSERT(m_semaphore != VK_NULL_HANDLE);

        m_graphics_device.set_object_name(m_semaphore, ObjectType::Semaphore, "Upload");

        HE_TRACE("Created Upload Semaphore");
    }

    VulkanUploadManager::~VulkanUploadManager()
    {
        vkDestroySemaphore(m_graphics_device.device(), m_semaphore, nullptr);
        vkDestroyCommandPool(m_graphics_device.device(), m_command_pool, nullptr);
    }

    void VulkanUploadManager::upload(const std::vector<BufferWrite> &buffer_writes)
    {
        const std::shared_ptr<VulkanQueue> queue = std::dynamic_pointer_cast<VulkanQueue>(m_graphics_device.queue());

        // TODO: Add texture uploads

        HE_VK_CHECK(vkResetCommandBuffer(m_command_buffer, 0));

        constexpr VkCommandBufferBeginInfo command_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr,
        };

        HE_VK_CHECK(vkBeginCommandBuffer(m_command_buffer, &command_buffer_begin_info));

        for (const BufferWrite &buffer_write : buffer_writes)
        {
            const std::shared_ptr<VulkanBuffer> buffer = std::dynamic_pointer_cast<VulkanBuffer>(buffer_write.handle);
            if (buffer->byte_size() <= 65535)
            {
                vkCmdUpdateBuffer(m_command_buffer, buffer->buffer(), 0, buffer_write.bytes.size(), buffer_write.bytes.data());
                HE_TRACE(
                    "Uploading {} bytes to {} buffer",
                    buffer_write.bytes.size(),
                    buffer->label().empty() ? "a" : fmt::format("the '{}'", buffer->label()));
            }
            else
            {
                // TODO: Staging Buffer
                HE_WARN(
                    "Tried to upload {} bytes to {} buffer, but staging buffers are not implemented",
                    buffer_write.bytes.size(),
                    buffer->label().empty() ? "a" : fmt::format("the '{}'", buffer->label()));
            }
        }

        HE_VK_CHECK(vkEndCommandBuffer(m_command_buffer));

        const uint64_t value = m_value++;

        const VkCommandBufferSubmitInfo command_buffer_submit_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBuffer = m_command_buffer,
            .deviceMask = 0,
        };

        const VkSemaphoreSubmitInfo semaphore_wait_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
            .pNext = nullptr,
            .semaphore = m_semaphore,
            .value = value,
            .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
            .deviceIndex = 0,
        };

        const VkSemaphoreSubmitInfo semaphore_signal_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
            .pNext = nullptr,
            .semaphore = m_semaphore,
            .value = value + 1,
            .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
            .deviceIndex = 0,
        };

        const VkSubmitInfo2 submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
            .pNext = nullptr,
            .flags = 0,
            .waitSemaphoreInfoCount = 1,
            .pWaitSemaphoreInfos = &semaphore_wait_info,
            .commandBufferInfoCount = 1,
            .pCommandBufferInfos = &command_buffer_submit_info,
            .signalSemaphoreInfoCount = 1,
            .pSignalSemaphoreInfos = &semaphore_signal_info,
        };

        HE_VK_CHECK(vkQueueSubmit2(queue->queue(), 1, &submit_info, VK_NULL_HANDLE));

        const uint64_t wait_value = value + 1;
        const VkSemaphoreWaitInfo wait_info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
            .pNext = nullptr,
            .flags = 0,
            .semaphoreCount = 1,
            .pSemaphores = &m_semaphore,
            .pValues = &wait_value,
        };

        HE_VK_CHECK(vkWaitSemaphores(m_graphics_device.device(), &wait_info, std::numeric_limits<uint64_t>::max()));
    }
} // namespace hyper_rhi