/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/render_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanRenderPipeline final : public IRenderPipeline
    {
    public:
        VulkanRenderPipeline(VulkanGraphicsDevice &graphics_device, const RenderPipelineDescriptor &descriptor);
        ~VulkanRenderPipeline() override;

        std::string_view label() const override;

        const std::shared_ptr<IPipelineLayout> &layout() const override;
        const std::shared_ptr<IShaderModule> &vertex_shader() const override;
        const std::shared_ptr<IShaderModule> &fragment_shader() const override;
        const std::vector<ColorAttachmentState> &color_attachment_states() const override;
        PrimitiveState primitive_state() const override;
        DepthStencilState depth_stencil_state() const override;

        VkPipeline pipeline() const;

        static VkPrimitiveTopology get_primitive_topology(PrimitiveTopology primitive_topology);
        static VkPolygonMode get_polygon_mode(PolygonMode polygon_mode);
        static VkCullModeFlags get_cull_mode_flags(Face face);
        static VkFrontFace get_front_face(FrontFace front_face);
        static VkCompareOp get_compare_operation(CompareOperation compare_operation);
        static VkBlendFactor get_blend_factor(BlendFactor blend_factor);
        static VkBlendOp get_blend_operation(BlendOperation blend_operation);
        static VkColorComponentFlags get_color_component_flags(ColorWrites color_writes);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        std::shared_ptr<IPipelineLayout> m_layout;
        std::shared_ptr<IShaderModule> m_vertex_shader;
        std::shared_ptr<IShaderModule> m_fragment_shader;
        std::vector<ColorAttachmentState> m_color_attachment_states;
        PrimitiveState m_primitive_state;
        DepthStencilState m_depth_stencil_state;

        VkPipeline m_pipeline;
    };
} // namespace hyper_engine