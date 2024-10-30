/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/render_pass.hpp"

#include <utility>

namespace hyper_rhi
{
    RenderPass::RenderPass(const RenderPassDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_label_color(descriptor.label_color)
    {
    }
} // namespace hyper_rhi