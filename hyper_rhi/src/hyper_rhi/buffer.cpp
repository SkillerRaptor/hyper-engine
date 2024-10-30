/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/buffer.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_rhi
{
    Buffer::Buffer(const BufferDescriptor &descriptor)
        : Resource(descriptor.label)
        , m_byte_size(descriptor.byte_size)
        , m_handle(descriptor.handle)
    {
        HE_ASSERT(descriptor.byte_size > 0);
    }

    uint64_t Buffer::byte_size() const
    {
        return m_byte_size;
    }

    ResourceHandle Buffer::handle() const
    {
        return m_handle;
    }
} // namespace hyper_rhi