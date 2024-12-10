/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/label_color.hpp"

namespace hyper_engine
{
    class ComputePipeline;

    struct ComputePassDescriptor
    {
        std::string label;
        LabelColor label_color;
    };

    class ComputePass
    {
    public:
        virtual ~ComputePass() = default;

        virtual void set_pipeline(const std::shared_ptr<ComputePipeline> &pipeline) = 0;
        virtual void set_push_constants(const void *data, size_t data_size) const = 0;

        virtual void dispatch(uint32_t x, uint32_t y, uint32_t z) const = 0;

        // TODO: Add indirect

        virtual std::string_view label() const = 0;
        virtual LabelColor label_color() const = 0;
    };
} // namespace hyper_engine
