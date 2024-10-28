/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/compute_pipeline.hpp"

#include <utility>

namespace hyper_rhi
{
    ComputePipeline::ComputePipeline(std::string label)
        : Resource(std::move(label))
    {
    }
} // namespace hyper_rhi