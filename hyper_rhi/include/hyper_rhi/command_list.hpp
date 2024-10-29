/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/render_pass.hpp"

namespace hyper_rhi
{
    class CommandList
    {
    public:
        virtual ~CommandList() = default;

        virtual void begin() = 0;
        virtual void end() = 0;

        [[nodiscard]] virtual RenderPassHandle begin_render_pass(const RenderPassDescriptor &descriptor) const = 0;
    };

    using CommandListHandle = std::shared_ptr<CommandList>;
} // namespace hyper_rhi
