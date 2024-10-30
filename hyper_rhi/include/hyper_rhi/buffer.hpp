/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include <hyper_core/bitmask.hpp>

#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_rhi
{
    enum class BufferUsageFlags : uint8_t
    {
        None = 0,
        ShaderResource = 1 << 0,
        IndexBuffer = 1 << 1,
    };

    HE_ENABLE_BITMASK_OPERATORS(BufferUsageFlags);

    struct BufferDescriptor
    {
        std::string label;

        uint64_t byte_size = 0;

        BufferUsageFlags usage = BufferUsageFlags::None;

        ResourceHandle handle;
    };

    class Buffer : public Resource
    {
    public:
        virtual ~Buffer() = default;

        [[nodiscard]] uint64_t byte_size() const;

        [[nodiscard]] ResourceHandle handle() const;

    protected:
        explicit Buffer(const BufferDescriptor &descriptor);

    protected:
        uint64_t m_byte_size;
        ResourceHandle m_handle;
    };

    using BufferHandle = std::shared_ptr<Buffer>;
} // namespace hyper_rhi
