/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/resource_handle.hpp"

namespace hyper_engine
{
    ResourceHandle::ResourceHandle()
        : m_handle(0xffffffff)
    {
    }

    ResourceHandle::ResourceHandle(const uint32_t handle)
        : m_handle(handle)
    {
    }

    bool ResourceHandle::valid() const
    {
        return m_handle != 0xffffffff;
    }

    uint32_t ResourceHandle::handle() const
    {
        return m_handle;
    }
} // namespace hyper_engine