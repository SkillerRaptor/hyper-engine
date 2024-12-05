/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "hyper_rhi/label_color.hpp"
#include "hyper_rhi/graphics_device.hpp"
#include "hyper_rhi/resource_handle.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

#include <vk_mem_alloc.h>

namespace hyper_engine
{
    class VulkanDescriptorManager;

    enum class ObjectType
    {
        Buffer,
        CommandPool,
        ComputePipeline,
        ComputeShaderModule,
        Fence,
        FragmentShaderModule,
        GraphicsPipeline,
        Image,
        ImageView,
        PipelineLayout,
        Queue,
        Sampler,
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
        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        ResourceHandle handle = {};
    };

    struct TextureEntry
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    struct TextureViewEntry
    {
        VkImageView view = VK_NULL_HANDLE;
        ResourceHandle handle = {};
    };

    struct ResourceQueue
    {
        std::vector<BufferEntry> buffers;
        std::vector<VkPipeline> compute_pipelines;
        std::vector<VkPipeline> graphics_pipelines;
        std::vector<VkPipelineLayout> pipeline_layouts;
        std::vector<VkSampler> samplers;
        std::vector<VkShaderModule> shader_modules;
        std::vector<TextureEntry> textures;
        std::vector<TextureViewEntry> texture_views;
    };

    struct FrameData
    {
        VkCommandPool command_pool;
        VkCommandBuffer command_buffer;

        VkFence render_fence;
        VkSemaphore submit_semaphore;
        uint64_t semaphore_counter;
    };

    class VulkanGraphicsDevice final : public IGraphicsDevice
    {
    public:
        explicit VulkanGraphicsDevice(const GraphicsDeviceDescriptor &descriptor);
        ~VulkanGraphicsDevice() override;

        std::shared_ptr<ISurface> create_surface() override;
        std::shared_ptr<IBuffer> create_buffer(const BufferDescriptor &descriptor) override;
        std::shared_ptr<IBuffer> create_staging_buffer(const BufferDescriptor &descriptor);
        std::shared_ptr<ICommandList> create_command_list() override;
        std::shared_ptr<IComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) override;
        std::shared_ptr<IRenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) override;
        std::shared_ptr<IPipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) override;
        std::shared_ptr<ISampler> create_sampler(const SamplerDescriptor &descriptor) override;
        std::shared_ptr<IShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) override;
        std::shared_ptr<ITexture> create_texture(const TextureDescriptor &descriptor) override;
        std::shared_ptr<ITextureView> create_texture_view(const TextureViewDescriptor &descriptor) override;

        std::shared_ptr<IImGuiManager> create_imgui_manager() override;

        void begin_marker(VkCommandBuffer command_buffer, MarkerType type, std::string_view name, LabelColor color) const;
        void end_marker(VkCommandBuffer command_buffer) const;

        void set_object_name(const void *handle, ObjectType type, std::string_view name) const;
        void destroy_resources();

        void begin_frame(const std::shared_ptr<ISurface> &surface, uint32_t frame_index) override;
        void end_frame() const override;
        void execute(const std::shared_ptr<ICommandList> &command_list) override;
        void present(const std::shared_ptr<ISurface> &surface) const override;

        void wait_for_idle() const override;

        GraphicsApi graphics_api() const override;
        bool debug_validation() const override;
        bool debug_label() const override;
        bool debug_marker() const override;

        VkInstance instance() const;
        VkPhysicalDevice physical_device() const;
        VkDevice device() const;
        uint32_t queue_family() const;
        VkQueue queue() const;
        VmaAllocator allocator() const;
        VulkanDescriptorManager &descriptor_manager() const;
        ResourceQueue &resource_queue();
        const FrameData &current_frame() const;
        uint32_t current_frame_index() const;

    private:
        void create_instance();
        void create_debug_messenger();
        void choose_physical_device();
        uint32_t rate_physical_device(const VkPhysicalDevice &physical_device) const;
        std::optional<uint32_t> find_queue_family(const VkPhysicalDevice &physical_device) const;
        void create_device();
        void create_allocator();
        void create_frames();

        static bool check_validation_layer_support();
        static bool check_extension_support(const VkPhysicalDevice &physical_device);
        static bool check_feature_support(const VkPhysicalDevice &physical_device);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_type,
            const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
            void *);

    private:
        GraphicsApi m_graphics_api;
        bool m_debug_validation;
        bool m_debug_label;
        bool m_debug_marker;

        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debug_messenger;
        VkPhysicalDevice m_physical_device;
        VkDevice m_device;
        uint32_t m_queue_family;
        VkQueue m_queue;
        VmaAllocator m_allocator;

        // NOTE: Using raw pointer to guarantee order of destruction
        VulkanDescriptorManager *m_descriptor_manager;

        uint32_t m_current_frame_index;
        std::array<FrameData, IGraphicsDevice::s_frame_count> m_frames;

        ResourceQueue m_resource_queue;
    };
} // namespace hyper_engine
