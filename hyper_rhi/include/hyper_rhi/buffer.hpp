/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/bits.hpp>

#include "hyper_rhi/resource_handle.hpp"

namespace hyper_engine
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

    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;

        virtual std::string_view label() const = 0;

        virtual uint64_t byte_size() const = 0;
        virtual BufferUsage usage() const = 0;

        virtual ResourceHandle handle() const = 0;
    };
} // namespace hyper_engine
