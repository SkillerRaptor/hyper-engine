/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/compute_pipeline.hpp"
#include "hyper_rhi/pass.hpp"

namespace hyper_rhi
{
    struct ComputePassDescriptor
    {
        std::string label;
        LabelColor label_color;
    };

    class ComputePass : public Pass
    {
    public:
        virtual ~ComputePass() = default;

        virtual void set_pipeline(const std::shared_ptr<ComputePipeline> &pipeline) = 0;
        virtual void set_push_constants(const void *data, size_t data_size) const = 0;

        virtual void dispatch(uint32_t x, uint32_t y, uint32_t z) const = 0;

        // TODO: Add indirect

    protected:
        explicit ComputePass(const ComputePassDescriptor &descriptor)
            : Pass(descriptor.label, descriptor.label_color)
        {
        }
    };
} // namespace hyper_rhi
