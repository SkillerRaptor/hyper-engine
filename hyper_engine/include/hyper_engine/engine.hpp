/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace hyper_engine
{
    class Engine
    {
    public:
        virtual ~Engine() = default;

        virtual bool initialize() = 0;
        virtual void shutdown() = 0;

        virtual void fixed_update(float delta_time, float total_time) = 0;
        virtual void update(float delta_time, float total_time) = 0;
        virtual void render() = 0;
    };
} // namespace hyper_engine