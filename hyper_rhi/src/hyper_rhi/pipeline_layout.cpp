/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/pipeline_layout.hpp"

#include <utility>

namespace hyper_rhi
{
    PipelineLayout::PipelineLayout(std::string label)
        : Resource(std::move(label))
    {
    }
} // namespace hyper_rhi