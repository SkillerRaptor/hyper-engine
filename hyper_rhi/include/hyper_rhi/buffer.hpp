/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_rhi
{
    struct BufferDescriptor
    {
        std::string label;

        uint64_t byte_size = 0;
        bool is_index_buffer = false;
        bool is_constant_buffer = false;
        bool has_index = false;

        // TODO: Add volatile buffers
    };

    class Buffer : public Resource
    {
    public:
        virtual ~Buffer() = default;

        [[nodiscard]] ResourceHandle handle() const;

    protected:
        explicit Buffer(std::string label);

    protected:
        ResourceHandle m_handle;
    };

    using BufferHandle = std::shared_ptr<Buffer>;
} // namespace hyper_rhi
