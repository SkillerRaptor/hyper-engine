/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>
#include <stack>

namespace hyper_engine
{
    class IEngine;
    class IModule;
    class WindowCloseEvent;

    class EngineLoop
    {
    public:
        EngineLoop();
        ~EngineLoop();

        bool pre_initialize(int32_t argc, const char **argv);
        bool initialize();

        void run();

    private:
        void load_module(std::unique_ptr<IModule> module);
        void unload_modules();

        void on_close(const WindowCloseEvent &event);

    private:
        std::stack<std::unique_ptr<IModule>> m_modules;

        bool m_editor_enabled;
        std::unique_ptr<IEngine> m_engine;

        bool m_exit_requested;
    };
} // namespace hyper_engine