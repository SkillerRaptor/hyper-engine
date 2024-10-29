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

    class VulkanQueue final : public Queue
    {
    public:
        explicit VulkanQueue(VulkanGraphicsDevice &graphics_device, uint32_t queue_family, VkQueue queue);
        ~VulkanQueue() override;

        void submit(CommandListHandle command_list_handle) override;

        [[nodiscard]] uint32_t queue_family() const;
        [[nodiscard]] VkQueue queue() const;
        [[nodiscard]] VkSemaphore submit_semaphore() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        uint32_t m_queue_family;
        VkQueue m_queue;
        VkSemaphore m_submit_semaphore;
    };
} // namespace hyper_rhi