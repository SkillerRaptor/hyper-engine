/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_core/logger.hpp"

#include <memory>

#include <fmt/color.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/ansicolor_sink-inl.h>

namespace he::core::logger
{
    static std::unique_ptr<spdlog::logger> g_logger = nullptr;

    void initialize()
    {
        const auto stdout_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        stdout_sink->set_color(spdlog::level::info, "\033[38;2;0;128;0m");
        stdout_sink->set_color(spdlog::level::warn, "\033[38;2;255;215;0m");
        stdout_sink->set_color(spdlog::level::err, "\033[38;2;255;0;0m");
        stdout_sink->set_color(spdlog::level::critical, "\033[38;2;220;20;60m");
        stdout_sink->set_color(spdlog::level::debug, "\033[38;2;0;0;255m");
        stdout_sink->set_color(spdlog::level::trace, "\033[38;2;128;0;128m");

        stdout_sink->set_pattern("\033[38;2;69;69;69m%Y-%m-%dT%H:%M:%S.%f %^%l%$ \033[38;2;120;120;120m%s:%#: \033[38;2;211;211;211m%v");

        const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("latest.log", true);
        file_sink->set_pattern("%Y-%m-%d%H:%M:%S.%f %l %s:%#: %v");

        g_logger = std::make_unique<spdlog::logger>(
            "HyperEngine",
            spdlog::sinks_init_list{
                stdout_sink,
                file_sink,
            });
        g_logger->set_level(spdlog::level::info);
        g_logger->flush_on(spdlog::level::info);
    }

    void shutdown()
    {
        g_logger = nullptr;
    }

    void set_level(const spdlog::level::level_enum level)
    {
        g_logger->set_level(level);
    }

    const std::unique_ptr<spdlog::logger> &internal_logger()
    {
        return g_logger;
    }
} // namespace he::core::logger
