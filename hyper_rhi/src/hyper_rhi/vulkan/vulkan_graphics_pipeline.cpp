/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_graphics_pipeline.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_pipeline_layout.hpp"
#include "hyper_rhi/vulkan/vulkan_shader_module.hpp"

namespace hyper_rhi
{
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanGraphicsDevice &graphics_device, const GraphicsPipelineDescriptor &descriptor)
        : GraphicsPipeline(descriptor)
        , m_graphics_device(graphics_device)
        , m_pipeline(VK_NULL_HANDLE)
    {
        const auto vertex_shader_module = std::dynamic_pointer_cast<VulkanShaderModule>(descriptor.vertex_shader);
        const auto fragment_shader_module = std::dynamic_pointer_cast<VulkanShaderModule>(descriptor.fragment_shader);
        const auto layout = std::dynamic_pointer_cast<VulkanPipelineLayout>(descriptor.layout);

        // TODO: Add to the descriptor a way to change that according to the images written to
        constexpr std::array<VkFormat, 1> color_attachment_formats = {
            VK_FORMAT_B8G8R8A8_SRGB,
        };

        const VkPipelineRenderingCreateInfo rendering_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
            .pNext = nullptr,
            .viewMask = 0,
            .colorAttachmentCount = static_cast<uint32_t>(color_attachment_formats.size()),
            .pColorAttachmentFormats = color_attachment_formats.data(),
            // TODO: Add to the descriptor a way to change that according to the depth image
            .depthAttachmentFormat = VK_FORMAT_D32_SFLOAT,
            .stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
        };

        const VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertex_shader_module->shader_module(),
            .pName = vertex_shader_module->entry_point().data(),
            .pSpecializationInfo = nullptr,
        };

        const VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragment_shader_module->shader_module(),
            .pName = fragment_shader_module->entry_point().data(),
            .pSpecializationInfo = nullptr,
        };

        const std::array<VkPipelineShaderStageCreateInfo, 2> shader_stage_create_infos = {
            vertex_shader_stage_create_info,
            fragment_shader_stage_create_info,
        };

        constexpr VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .vertexBindingDescriptionCount = 0,
            .pVertexBindingDescriptions = nullptr,
            .vertexAttributeDescriptionCount = 0,
            .pVertexAttributeDescriptions = nullptr,
        };

        constexpr VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = false,
        };

        constexpr VkPipelineTessellationStateCreateInfo tessellation_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .patchControlPoints = 0,
        };

        constexpr VkPipelineViewportStateCreateInfo viewport_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .viewportCount = 1,
            .pViewports = nullptr,
            .scissorCount = 1,
            .pScissors = nullptr,
        };

        constexpr VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = false,
            .rasterizerDiscardEnable = false,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = false,
            .depthBiasConstantFactor = 0.0,
            .depthBiasClamp = 0.0,
            .depthBiasSlopeFactor = 0.0,
            .lineWidth = 1.0,
        };

        constexpr VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = false,
            .minSampleShading = 1.0,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = false,
            .alphaToOneEnable = false,
        };

        const bool depth_enabled = descriptor.depth_state.depth_enabled;
        const VkCompareOp compare_operation = VulkanGraphicsPipeline::get_compare_operation(descriptor.depth_state.compare_operation);

        const VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthTestEnable = depth_enabled,
            .depthWriteEnable = depth_enabled,
            .depthCompareOp = compare_operation,
            .depthBoundsTestEnable = false,
            .stencilTestEnable = false,
            .front = {},
            .back = {},
            .minDepthBounds = 0.0,
            .maxDepthBounds = 1.0,
        };

        constexpr VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
            .blendEnable = true,
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        const VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_NO_OP,
            .attachmentCount = 1,
            .pAttachments = &color_blend_attachment_state,
            .blendConstants = {
                0.0,
                0.0,
                0.0,
                0.0,
            },
        };

        constexpr std::array<VkDynamicState, 2> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };

        const VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
            .pDynamicStates = dynamic_states.data(),
        };

        const VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = &rendering_create_info,
            .flags = 0,
            .stageCount = static_cast<uint32_t>(shader_stage_create_infos.size()),
            .pStages = shader_stage_create_infos.data(),
            .pVertexInputState = &vertex_input_state_create_info,
            .pInputAssemblyState = &input_assembly_state_create_info,
            .pTessellationState = &tessellation_state_create_info,
            .pViewportState = &viewport_state_create_info,
            .pRasterizationState = &rasterization_state_create_info,
            .pMultisampleState = &multisample_state_create_info,
            .pDepthStencilState = &depth_stencil_state_create_info,
            .pColorBlendState = &color_blend_state_create_info,
            .pDynamicState = &dynamic_state_create_info,
            .layout = layout->pipeline_layout(),
            .renderPass = VK_NULL_HANDLE,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1,
        };

        HE_VK_CHECK(
            vkCreateGraphicsPipelines(m_graphics_device.device(), VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, nullptr, &m_pipeline));
        HE_ASSERT(m_pipeline != VK_NULL_HANDLE);

        if (!m_label.empty())
        {
            m_graphics_device.set_object_name(m_pipeline, VK_OBJECT_TYPE_PIPELINE, m_label);
        }

        if (descriptor.layout->label().empty() && descriptor.vertex_shader->label().empty() && descriptor.fragment_shader->label().empty())
        {
            HE_TRACE("Created Graphics Pipeline {}", m_label.empty() ? "" : fmt::format("'{}'", m_label));
        }
        else
        {
            std::string message;

            size_t index = 0;
            std::array<std::string, 3> parts = {};
            if (!descriptor.layout->label().empty())
            {
                parts[index] = fmt::format("'{}' layout", descriptor.layout->label());
                ++index;
            }

            if (!descriptor.vertex_shader->label().empty())
            {
                parts[index] = fmt::format("'{}' vertex shader", descriptor.vertex_shader->label());
                ++index;
            }

            if (!descriptor.fragment_shader->label().empty())
            {
                parts[index] = fmt::format("'{}' fragment shader", descriptor.fragment_shader->label());
            }

            switch (index)
            {
            case 0:
                message += parts[0];
                break;
            case 1:
                message += fmt::format("{} and {}", parts[0], parts[1]);
                break;
            case 2:
                message += fmt::format("{}, {} and {}", parts[0], parts[1], parts[2]);
                break;
            default:
                HE_UNREACHABLE();
            }

            HE_TRACE("Created Graphics Pipeline {} with {}", m_label.empty() ? "" : fmt::format("'{}'", m_label), message);
        }
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
    {
        m_graphics_device.resource_queue().graphics_pipelines.emplace_back(m_pipeline);
    }

    VkPipeline VulkanGraphicsPipeline::pipeline() const
    {
        return m_pipeline;
    }

    VkCompareOp VulkanGraphicsPipeline::get_compare_operation(const CompareOperation compare_operation)
    {
        switch (compare_operation)
        {
        case CompareOperation::Never:
            return VK_COMPARE_OP_NEVER;
        case CompareOperation::Less:
            return VK_COMPARE_OP_LESS;
        case CompareOperation::Equal:
            return VK_COMPARE_OP_EQUAL;
        case CompareOperation::LessEqual:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOperation::Greater:
            return VK_COMPARE_OP_GREATER;
        case CompareOperation::NotEqual:
            return VK_COMPARE_OP_NOT_EQUAL;
        case CompareOperation::GreaterEqual:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareOperation::Always:
            return VK_COMPARE_OP_ALWAYS;
        case CompareOperation::Undefined:
        default:
            HE_UNREACHABLE();
        }
    }
} // namespace hyper_rhi