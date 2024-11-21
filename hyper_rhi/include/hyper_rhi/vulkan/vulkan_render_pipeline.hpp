/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/render_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanRenderPipeline final : public IRenderPipeline
    {
    public:
        VulkanRenderPipeline(VulkanGraphicsDevice &graphics_device, const RenderPipelineDescriptor &descriptor);
        ~VulkanRenderPipeline() override;

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

        VkPipeline m_pipeline;
    };
} // namespace he::rhi