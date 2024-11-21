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
    class IComputePipeline;

    struct ComputePassDescriptor
    {
        std::string label;
        LabelColor label_color;
    };

    class IComputePass
    {
    public:
        virtual ~IComputePass() = default;

        virtual void set_pipeline(const std::shared_ptr<IComputePipeline> &pipeline) = 0;
        virtual void set_push_constants(const void *data, size_t data_size) const = 0;

        virtual void dispatch(uint32_t x, uint32_t y, uint32_t z) const = 0;

        // TODO: Add indirect

        std::string_view label() const;
        LabelColor label_color() const;

    protected:
        explicit IComputePass(const ComputePassDescriptor &descriptor);

    protected:
        std::string m_label;
        LabelColor m_label_color;
    };
} // namespace hyper_engine
