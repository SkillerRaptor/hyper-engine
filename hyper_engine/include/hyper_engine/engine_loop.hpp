/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/own_ptr.hpp>
#include <hyper_platform/forward.hpp>

namespace hyper_engine
{
    class Engine;

    class EngineLoop
    {
    public:
        EngineLoop();
        ~EngineLoop();

        bool pre_initialize(int32_t argc, const char **argv);
        bool initialize();

        void run();

    private:
        void on_close(const WindowCloseEvent &event);

    private:
        bool m_editor_enabled = false;
        OwnPtr<Engine> m_engine;
        bool m_exit_requested = false;
    };
} // namespace hyper_engine