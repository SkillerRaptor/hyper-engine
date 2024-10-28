/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/render_pass.hpp"

#include <utility>

namespace hyper_rhi
{
    RenderPass::RenderPass(std::string label)
        : Resource(std::move(label))
    {
    }
} // namespace hyper_rhi