/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/bits.hpp>

#include "hyper_rhi/resource_handle.hpp"

namespace he::rhi
{
    enum class BufferUsage : uint8_t
    {
        None = 0,
        Index = 1 << 0,
        Indirect = 1 << 1,
        Storage = 1 << 2,
        ShaderResource = 1 << 3,
    };

    HE_ENABLE_BITMASK_OPERATORS(BufferUsage);

    struct BufferDescriptor
    {
        std::string label;

        uint64_t byte_size = 0;
        BufferUsage usage = BufferUsage::None;

        ResourceHandle handle = {};
    };

    class Buffer
    {
    public:
        virtual ~Buffer() = default;

        std::string_view label() const;

        uint64_t byte_size() const;
        BufferUsage usage() const;

        ResourceHandle handle() const;

    protected:
        explicit Buffer(const BufferDescriptor &descriptor);

    protected:
        std::string m_label;

        uint64_t m_byte_size;
        BufferUsage m_usage;

        ResourceHandle m_handle;
    };
} // namespace he::rhi
