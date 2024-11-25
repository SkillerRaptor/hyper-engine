/*
 * Copyright (c) 2024, SkillerRaptor
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
    class Renderer;
    class Window;

    // NOTE: Using raw pointers, to ensure that variables are only deleted when the module is unloaded
    struct GlobalEnvironment
    {
        Logger *logger;
        JobSystem *job_system;
        EventBus *event_bus;
        Input *input;
        Window *window;
        Renderer *renderer;
    };

    extern GlobalEnvironment g_environment;
} // namespace hyper_engine