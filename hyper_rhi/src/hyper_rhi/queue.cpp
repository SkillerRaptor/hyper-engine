/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/queue.hpp"

namespace hyper_rhi
{
    void Queue::write_buffer(const std::shared_ptr<Buffer> &buffer, const void *data, const size_t size)
    {
        const auto *byte_data = static_cast<const uint8_t *>(data);
        const std::vector<uint8_t> bytes(byte_data, byte_data + size);

        m_buffer_writes.push_back({
            .buffer = buffer,
            .bytes = bytes,
        });
    }
} // namespace hyper_rhi