/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <vector>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/command_list.hpp"

namespace hyper_rhi
{
    struct BufferWrite
    {
        BufferHandle handle;
        std::vector<uint8_t> bytes;
    };

    class Queue
    {
    public:
        virtual ~Queue() = default;

        void write_buffer(const BufferHandle &buffer, const void *data, size_t size);

        virtual void submit(CommandListHandle command_list_handle) = 0;

    protected:
        std::vector<BufferWrite> m_buffer_writes;
    };

    using QueueHandle = std::shared_ptr<Queue>;
} // namespace hyper_rhi