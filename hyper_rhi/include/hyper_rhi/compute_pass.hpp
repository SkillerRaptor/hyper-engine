/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/ref_counted.hpp>
#include <hyper_core/ref_ptr.hpp>

#include "hyper_rhi/forward.hpp"
#include "hyper_rhi/label_color.hpp"

namespace hyper_engine
{
    struct ComputePassDescriptor
    {
        std::string label;
        LabelColor label_color;
    };

    class ComputePass : public RefCounted<ComputePass>
    {
    public:
        virtual ~ComputePass() = default;

        // FIXME: This should be NonnullRefPtr
        virtual void set_pipeline(const RefPtr<ComputePipeline> &pipeline) = 0;
        virtual void set_push_constants(const void *data, size_t data_size) const = 0;

        virtual void dispatch(uint32_t x, uint32_t y, uint32_t z) const = 0;

        // FIXME: Add indirect

        std::string_view label() const;
        LabelColor label_color() const;

    protected:
        explicit ComputePass(const ComputePassDescriptor &descriptor);

    protected:
        std::string m_label;
        LabelColor m_label_color;
    };
} // namespace hyper_engine
