/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/resource.hpp"

namespace hyper_rhi
{
    struct PipelineLayoutDescriptor
    {
        std::string label;

        uint32_t push_constant_size = 0;
    };

    class PipelineLayout : public Resource
    {
    public:
        virtual ~PipelineLayout() = default;

        [[nodiscard]] HE_FORCE_INLINE uint32_t push_constant_size() const
        {
            return m_push_constant_size;
        }

    protected:
        explicit PipelineLayout(const PipelineLayoutDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_push_constant_size(descriptor.push_constant_size)
        {
        }

    protected:
        uint32_t m_push_constant_size;
    };
} // namespace hyper_rhi
