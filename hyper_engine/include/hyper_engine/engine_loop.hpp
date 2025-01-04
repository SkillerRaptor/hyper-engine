/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>
#include <stack>

#include <hyper_core/nonnull_own_ptr.hpp>
#include <hyper_core/own_ptr.hpp>
#include <hyper_platform/forward.hpp>

namespace hyper_engine
{
    class Engine;
    class Module;

    class EngineLoop
    {
    public:
        EngineLoop();
        ~EngineLoop();

        bool pre_initialize(int32_t argc, const char **argv);
        bool initialize();

        void run();

    private:
        void load_module(NonnullOwnPtr<Module> module);
        void unload_modules();

        void on_close(const WindowCloseEvent &event);

    private:
        std::stack<NonnullOwnPtr<Module>> m_modules;

        bool m_editor_enabled;
        OwnPtr<Engine> m_engine;

        bool m_exit_requested;
    };
} // namespace hyper_engine