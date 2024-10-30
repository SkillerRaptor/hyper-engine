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
        std::shared_ptr<Buffer> buffer;
        std::vector<uint8_t> bytes;
    };

    class Queue
    {
    public:
        virtual ~Queue() = default;

        void write_buffer(const std::shared_ptr<Buffer> &buffer, const void *data, size_t size);

        virtual void submit(const std::shared_ptr<CommandList> &command_list) = 0;

    protected:
        std::vector<BufferWrite> m_buffer_writes;
    };
} // namespace hyper_rhi