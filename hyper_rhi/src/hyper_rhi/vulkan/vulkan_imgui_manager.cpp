/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_imgui_manager.hpp"

#include <volk.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "hyper_rhi/vulkan/vulkan_command_list.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_texture.hpp"

namespace hyper_rhi
{
    VulkanImGuiManager::VulkanImGuiManager(VulkanGraphicsDevice &graphics_device)
        : m_graphics_device(graphics_device)
    {
    }

    void VulkanImGuiManager::initialize(const hyper_platform::Window &window, const std::shared_ptr<Surface> &surface)
    {
        const VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 },
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
        pool_info.pPoolSizes = pool_sizes;

        VkDescriptorPool imguiPool;
        HE_VK_CHECK(vkCreateDescriptorPool(m_graphics_device.device(), &pool_info, nullptr, &imguiPool));

        const Format format = surface->format();
        const VkFormat vk_format = VulkanTexture::get_format(format);

        ImGui_ImplGlfw_InitForVulkan(window.native_window(), true);

        ImGui_ImplVulkan_InitInfo init_info = {
            .Instance = m_graphics_device.instance(),
            .PhysicalDevice = m_graphics_device.physical_device(),
            .Device = m_graphics_device.device(),
            .QueueFamily = m_graphics_device.queue_family(),
            .Queue = m_graphics_device.queue(),
            .DescriptorPool = imguiPool,
            .RenderPass = VK_NULL_HANDLE,
            .MinImageCount = surface->min_image_count(),
            .ImageCount = surface->image_count(),
            .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
            .PipelineCache = VK_NULL_HANDLE,
            .Subpass = 0,
            .UseDynamicRendering = true,
            .PipelineRenderingCreateInfo =
                VkPipelineRenderingCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                    .pNext = nullptr,
                    .viewMask = 0,
                    .colorAttachmentCount = 1,
                    .pColorAttachmentFormats = &vk_format,
                    .depthAttachmentFormat = VK_FORMAT_UNDEFINED,
                    .stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
                },
            .Allocator = nullptr,
            .CheckVkResultFn = nullptr,
            .MinAllocationSize = 0,
        };

        auto function_loader = [](const char *function_name, void *user_data)
        {
            const auto *graphics_device = static_cast<VulkanGraphicsDevice *>(user_data);
            return vkGetInstanceProcAddr(graphics_device->instance(), function_name);
        };

        ImGui_ImplVulkan_LoadFunctions(function_loader, &m_graphics_device);
        ImGui_ImplVulkan_Init(&init_info);

        ImGui_ImplVulkan_CreateFontsTexture();
    }

    void VulkanImGuiManager::destroy()
    {
        ImGui_ImplVulkan_Shutdown();
    }

    void VulkanImGuiManager::new_frame()
    {
        ImGui_ImplVulkan_NewFrame();
    }

    void VulkanImGuiManager::render(const std::shared_ptr<CommandList> &command_list)
    {
        const auto vulkan_command_list = std::dynamic_pointer_cast<VulkanCommandList>(command_list);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vulkan_command_list->command_buffer());
    }

    void VulkanImGuiManager::on_resize(const uint32_t width, const uint32_t height)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }
} // namespace hyper_rhi