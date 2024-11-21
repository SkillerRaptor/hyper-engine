/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/buffer.hpp"

#include <hyper_core/assertion.hpp>

namespace hyper_engine
{
    IBuffer::IBuffer(const BufferDescriptor &descriptor)
        : m_label(descriptor.label)
        , m_byte_size(descriptor.byte_size)
        , m_usage(descriptor.usage)
        , m_handle(descriptor.handle)
    {
        HE_ASSERT(!m_label.empty(), "Buffer wasn't given a label");

        HE_ASSERT(m_byte_size > 0);
        HE_ASSERT(m_usage != BufferUsage::None);

        if ((m_usage & BufferUsage::ShaderResource) == BufferUsage::ShaderResource)
        {
            HE_ASSERT((m_usage & BufferUsage::Storage) == BufferUsage::Storage);
        }
    }

    std::string_view IBuffer::label() const
    {
        return m_label;
    }

    uint64_t IBuffer::byte_size() const
    {
        return m_byte_size;
    }

    BufferUsage IBuffer::usage() const
    {
        return m_usage;
    }

    ResourceHandle IBuffer::handle() const
    {
        return m_handle;
    }
} // namespace hyper_engine