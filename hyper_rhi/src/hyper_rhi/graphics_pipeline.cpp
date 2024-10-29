/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/graphics_pipeline.hpp"

namespace hyper_rhi
{
    GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_layout(descriptor.layout)
    {
    }

    PipelineLayoutHandle GraphicsPipeline::layout() const
    {
        return m_layout;
    }
} // namespace hyper_rhi