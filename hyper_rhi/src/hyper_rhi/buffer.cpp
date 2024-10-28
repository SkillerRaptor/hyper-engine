/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/buffer.hpp"

#include <utility>

namespace hyper_rhi
{
    Buffer::Buffer(std::string label)
        : Resource(std::move(label))
        , m_handle(ResourceHandle(0xffffffff))
    {
    }

    ResourceHandle Buffer::handle() const
    {
        return m_handle;
    }
} // namespace hyper_rhi