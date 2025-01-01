/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace hyper_engine
{
    class EventBus;
    class JobSystem;
    class Input;
    class Logger;
    class GraphicsDevice;
    class Renderer;
    class Window;

    // NOTE: Using raw pointers, to ensure that variables are only deleted when the module is unloaded
    struct GlobalEnvironment
    {
        // NOTE: Core Module
        Logger *logger;
        JobSystem *job_system;
        // NOTE: Event Module
        EventBus *event_bus;
        // NOTE: Platform Module
        Input *input;
        Window *window;
        // NOTE: Rhi Module
        GraphicsDevice *graphics_device;
        // NOTE: Renderer Module
        Renderer *renderer;
    };

    extern GlobalEnvironment g_env;
} // namespace hyper_engine