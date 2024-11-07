/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/assertion.hpp>
#include <hyper_core/bitmask.hpp>
#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_rhi
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

    class Buffer : public Resource
    {
    public:
        virtual ~Buffer() = default;

        [[nodiscard]] HE_FORCE_INLINE uint64_t byte_size() const
        {
            return m_byte_size;
        }

        [[nodiscard]] HE_FORCE_INLINE BufferUsage usage() const
        {
            return m_usage;
        }

        [[nodiscard]] HE_FORCE_INLINE ResourceHandle handle() const
        {
            return m_handle;
        }

    protected:
        explicit Buffer(const BufferDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_byte_size(descriptor.byte_size)
            , m_usage(descriptor.usage)
            , m_handle(descriptor.handle)
        {
            HE_ASSERT(descriptor.byte_size > 0);
            HE_ASSERT(descriptor.usage != BufferUsage::None);

            if ((descriptor.usage & BufferUsage::ShaderResource) == BufferUsage::ShaderResource)
            {
                HE_ASSERT((descriptor.usage & BufferUsage::Storage) == BufferUsage::Storage);
            }
        }

    protected:
        uint64_t m_byte_size;
        BufferUsage m_usage;

        ResourceHandle m_handle;
    };
} // namespace hyper_rhi
