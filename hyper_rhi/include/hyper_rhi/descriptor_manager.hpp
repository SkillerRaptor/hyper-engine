/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stack>

#include <hyper_core/ref_ptr.hpp>

#include "hyper_rhi/forward.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_engine
{
    class DescriptorManager
    {
    public:
        virtual ~DescriptorManager() = default;

        ResourceHandle allocate_handle();
        void retire_handle(ResourceHandle handle);

        virtual void set_buffer(const RefPtr<Buffer> &buffer, ResourceHandle handle) const = 0;
        virtual void set_storage_image(const RefPtr<TextureView> &texture_view, ResourceHandle handle) const = 0;
        virtual void set_sampled_image(const RefPtr<TextureView> &texture_view, ResourceHandle handle) const = 0;
        virtual void set_sampler(const RefPtr<Sampler> &sampler, ResourceHandle handle) const = 0;

    private:
        std::stack<ResourceHandle> m_recycled_descriptors;
        uint32_t m_current_index = 0;
    };
} // namespace hyper_engine