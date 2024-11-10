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

#include <vk_mem_alloc.h>

namespace hyper_rhi
{
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

    class VulkanGraphicsDevice final : public GraphicsDevice
    {
    public:
        explicit VulkanGraphicsDevice(const GraphicsDeviceDescriptor &descriptor);
        ~VulkanGraphicsDevice() override;

        std::shared_ptr<Surface> create_surface(const hyper_platform::Window &window) override;
        std::shared_ptr<Buffer> create_buffer(const BufferDescriptor &descriptor) override;
        std::shared_ptr<Buffer> create_staging_buffer(const BufferDescriptor &descriptor);
        std::shared_ptr<CommandList> create_command_list() override;
        std::shared_ptr<ComputePipeline> create_compute_pipeline(const ComputePipelineDescriptor &descriptor) override;
        std::shared_ptr<RenderPipeline> create_render_pipeline(const RenderPipelineDescriptor &descriptor) override;
        std::shared_ptr<PipelineLayout> create_pipeline_layout(const PipelineLayoutDescriptor &descriptor) override;
        std::shared_ptr<Sampler> create_sampler(const SamplerDescriptor &descriptor) override;
        std::shared_ptr<ShaderModule> create_shader_module(const ShaderModuleDescriptor &descriptor) override;
        std::shared_ptr<Texture> create_texture(const TextureDescriptor &descriptor) override;
        std::shared_ptr<TextureView> create_texture_view(const TextureViewDescriptor &descriptor) override;

        std::shared_ptr<ImGuiManager> create_imgui_manager() override;

        void begin_marker(VkCommandBuffer command_buffer, MarkerType type, std::string_view name, LabelColor color) const;
        void end_marker(VkCommandBuffer command_buffer) const;

        void set_object_name(const void *handle, ObjectType type, std::string_view name) const;
        void destroy_resources();

        void begin_frame(const std::shared_ptr<Surface> &surface, uint32_t frame_index) override;
        void end_frame() const override;
        void execute(const std::shared_ptr<CommandList> &command_list) override;
        void present(const std::shared_ptr<Surface> &surface) const override;

        void wait_for_idle() const override;

        [[nodiscard]] HE_FORCE_INLINE VkInstance instance() const
        {
            return m_instance;
        }

        [[nodiscard]] HE_FORCE_INLINE VkPhysicalDevice physical_device() const
        {
            return m_physical_device;
        }

        [[nodiscard]] HE_FORCE_INLINE VkDevice device() const
        {
            return m_device;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t queue_family() const
        {
            return m_queue_family;
        }

        [[nodiscard]] HE_FORCE_INLINE VkQueue queue() const
        {
            return m_queue;
        }

        [[nodiscard]] HE_FORCE_INLINE VmaAllocator allocator() const
        {
            return m_allocator;
        }

        [[nodiscard]] HE_FORCE_INLINE VulkanDescriptorManager &descriptor_manager() const
        {
            return *m_descriptor_manager;
        }

        [[nodiscard]] HE_FORCE_INLINE ResourceQueue &resource_queue()
        {
            return m_resource_queue;
        }

        [[nodiscard]] HE_FORCE_INLINE const FrameData &current_frame() const
        {
            return m_frames[m_current_frame_index % GraphicsDevice::s_frame_count];
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t current_frame_index() const
        {
            return m_current_frame_index;
        }

    private:
        void create_instance();
        void create_debug_messenger();
        void choose_physical_device();
        [[nodiscard]] uint32_t rate_physical_device(const VkPhysicalDevice &physical_device) const;
        [[nodiscard]] std::optional<uint32_t> find_queue_family(const VkPhysicalDevice &physical_device) const;
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
        std::array<FrameData, GraphicsDevice::s_frame_count> m_frames;

        ResourceQueue m_resource_queue;
    };
} // namespace hyper_rhi
