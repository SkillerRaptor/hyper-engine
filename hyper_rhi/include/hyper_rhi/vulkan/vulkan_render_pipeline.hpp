/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/render_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanRenderPipeline final : public RenderPipeline
    {
    public:
        VulkanRenderPipeline(VulkanGraphicsDevice &graphics_device, const RenderPipelineDescriptor &descriptor);
        ~VulkanRenderPipeline() override;

        [[nodiscard]] static VkPrimitiveTopology get_primitive_topology(PrimitiveTopology primitive_topology);
        [[nodiscard]] static VkPolygonMode get_polygon_mode(PolygonMode polygon_mode);
        [[nodiscard]] static VkCullModeFlags get_cull_mode_flags(Face face);
        [[nodiscard]] static VkFrontFace get_front_face(FrontFace front_face);
        [[nodiscard]] static VkCompareOp get_compare_operation(CompareOperation compare_operation);
        [[nodiscard]] static VkBlendFactor get_blend_factor(BlendFactor blend_factor);
        [[nodiscard]] static VkBlendOp get_blend_operation(BlendOperation blend_operation);
        [[nodiscard]] static VkColorComponentFlags get_color_component_flags(ColorWrites color_writes);

        [[nodiscard]] HE_FORCE_INLINE VkPipeline pipeline() const
        {
            return m_pipeline;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkPipeline m_pipeline;
    };
} // namespace hyper_rhi