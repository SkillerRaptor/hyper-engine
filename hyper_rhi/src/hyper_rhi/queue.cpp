/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/queue.hpp"

namespace hyper_rhi
{
    void Queue::write_buffer(const BufferHandle &buffer, const void *data, size_t size)
    {
        const auto *byte_data = static_cast<const uint8_t *>(data);
        std::vector<uint8_t> bytes(byte_data, byte_data + size);
        m_buffer_writes.emplace_back(buffer, bytes);
    }
} // namespace hyper_rhi