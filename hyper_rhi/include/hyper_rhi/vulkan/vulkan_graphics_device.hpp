/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <memory>
#include <optional>

#include "hyper_rhi/graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"
#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"
#include "hyper_rhi/vulkan/vulkan_queue.hpp"
#include "hyper_rhi/vulkan/vulkan_upload_manager.hpp"

#include <vk_mem_alloc.h>

namespace hyper_rhi
{
    enum class ObjectType
    {
        Buffer,
        CommandPool,
        ComputePipeline,
        ComputeShaderModule,
        FragmentShaderModule,
        GraphicsPipeline,
        Image,
        ImageView,
        PipelineLayout,
        Queue,
        Semaphore,
        VertexShaderModule,
    };

    enum class MarkerType
    {
        ComputePass,
        RenderPass
    };

    struct BufferEntry
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        ResourceHandle handle;
    };

    struct TextureEntry
    {
        VkImage image;
        VmaAllocation allocation;
        VkImageView view;
        ResourceHandle handle;
    };

    struct ResourceQueue
    {
        std::vector<BufferEntry> buffers;
        std::vector<VkPipeline> compute_pipelines;
        std::vector<VkPipeline> graphics_pipelines;
        std::vector<VkPipelineLayout> pipeline_layouts;
        std::vector<VkShaderModule> shader_modules;
        std::vector<TextureEntry> textures;
    };

    struct FrameData
    {
        VkCommandPool command_pool;
        VkCommandBuffer command_buffer;

        VkSemaphore render_semaphore;
        VkSemaphore present_semaphore;
    };

    class VulkanGraphicsDevice final : public GraphicsDevice
    {
    public:
        explicit VulkanGraphicsDevice(const GraphicsDeviceDescriptor &descriptor);
        ~VulkanGraphicsDevice() override;

        SurfaceHandle create_surface(const hyper_platform::Window &window) override;
        QueueHandle queue() override;

        BufferHandle create_buffer(const BufferDescriptor &descriptor) override;
        CommandListHandle create_command_list() override;
        ComputePipelineHandle create_compute_pipeline(const ComputePipelineDescriptor &descriptor) override;
        GraphicsPipelineHandle create_graphics_pipeline(const GraphicsPipelineDescriptor &descriptor) override;
        PipelineLayoutHandle create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) override;
        ShaderModuleHandle create_shader_module(const ShaderModuleDescriptor &descriptor) override;
        TextureHandle create_texture(const TextureDescriptor &descriptor) override;
        std::shared_ptr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor) override;

        void begin_marker(VkCommandBuffer command_buffer, MarkerType type, std::string_view name, LabelColor color) const;
        void end_marker(VkCommandBuffer command_buffer) const;

        void set_object_name(const void *handle, ObjectType type, std::string_view name) const;
        void destroy_resources();

        void begin_frame(SurfaceHandle surface_handle, uint32_t frame_index) override;
        void end_frame() const override;
        void present(SurfaceHandle surface_handle) const override;

        void wait_for_idle() const override;

        [[nodiscard]] VkInstance instance() const;
        [[nodiscard]] VkPhysicalDevice physical_device() const;
        [[nodiscard]] VkDevice device() const;
        [[nodiscard]] VmaAllocator allocator() const;
        [[nodiscard]] VulkanDescriptorManager &descriptor_manager() const;
        [[nodiscard]] VulkanUploadManager &upload_manager() const;
        [[nodiscard]] ResourceQueue &resource_queue();

        [[nodiscard]] const FrameData &current_frame() const;
        [[nodiscard]] uint32_t current_frame_index() const;

    private:
        static bool check_validation_layer_support();
        void create_instance();
        void create_debug_messenger();

        void choose_physical_device();
        uint32_t rate_physical_device(const VkPhysicalDevice &physical_device) const;
        std::optional<uint32_t> find_queue_family(const VkPhysicalDevice &physical_device) const;
        static bool check_extension_support(const VkPhysicalDevice &physical_device);
        static bool check_feature_support(const VkPhysicalDevice &physical_device);

        void create_device();
        void create_allocator();

        void create_frames();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_type,
            const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
            void *);

    private:
        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debug_messenger;
        VkPhysicalDevice m_physical_device;
        VkDevice m_device;
        std::shared_ptr<VulkanQueue> m_queue;
        VmaAllocator m_allocator;

        // NOTE: Using raw pointer to guarantee order of destruction
        VulkanDescriptorManager *m_descriptor_manager;
        VulkanUploadManager *m_upload_manager;

        std::array<FrameData, GraphicsDevice::s_frame_count> m_frames;
        uint32_t m_current_frame_index;

        ResourceQueue m_resource_queue;
    };
} // namespace hyper_rhi
