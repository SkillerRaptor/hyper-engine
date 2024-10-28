/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/graphics_pipeline.hpp"

#include <utility>

namespace hyper_rhi
{
    GraphicsPipeline::GraphicsPipeline(std::string label)
        : Resource(std::move(label))
    {
    }
} // namespace hyper_rhi