/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/command_list.hpp"

#include "hyper_rhi/compute_pass.hpp"
#include "hyper_rhi/render_pass.hpp"

namespace hyper_engine
{
    NonnullRefPtr<ComputePass> CommandList::begin_compute_pass(const ComputePassDescriptor &descriptor) const
    {
        return begin_compute_pass_platform(descriptor);
    }

    NonnullRefPtr<RenderPass> CommandList::begin_render_pass(const RenderPassDescriptor &descriptor) const
    {
        HE_ASSERT(!descriptor.color_attachments.empty());

        return begin_render_pass_platform(descriptor);
    }
} // namespace hyper_engine