/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace hyper_engine
{
    class IEngine
    {
    public:
        virtual ~IEngine() = default;

        virtual bool initialize() = 0;
        virtual void shutdown() = 0;

        virtual void fixed_update(float delta_time, float total_time) = 0;
        virtual void update(float delta_time, float total_time) = 0;
        virtual void render() = 0;
    };
} // namespace hyper_engine