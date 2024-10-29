/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/queue.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanUploadManager
    {
    public:
        explicit VulkanUploadManager(VulkanGraphicsDevice &graphics_device);
        ~VulkanUploadManager();

        void upload(const std::vector<BufferWrite> &buffer_writes);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandPool m_command_pool;
        VkCommandBuffer m_command_buffer;

        VkSemaphore m_semaphore;
        uint64_t m_value;
    };
} // namespace hyper_rhi