/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_core/logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/ansicolor_sink-inl.h>
#include <spdlog/fmt/bundled/color.h>

namespace hyper_core::logger
{
    void initialize()
    {
        const std::shared_ptr<spdlog::sinks::ansicolor_stdout_sink_mt> stdout_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        stdout_sink->set_color(spdlog::level::info, "\033[38;2;0;128;0m");
        stdout_sink->set_color(spdlog::level::warn, "\033[38;2;255;215;0m");
        stdout_sink->set_color(spdlog::level::err, "\033[38;2;255;0;0m");
        stdout_sink->set_color(spdlog::level::critical, "\033[38;2;220;20;60m");
        stdout_sink->set_color(spdlog::level::debug, "\033[38;2;0;0;255m");
        stdout_sink->set_color(spdlog::level::trace, "\033[38;2;128;0;128m");

        stdout_sink->set_pattern("\033[38;2;169;169;169m%Y-%m-%dT%H:%M:%S.%f %^%l%$ \033[38;2;211;211;211m%v");

        const std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("latest.log", true);
        file_sink->set_pattern("%Y-%m-%d%H:%M:%S.%f %l: %v");

        const spdlog::sinks_init_list log_sinks = {
            stdout_sink,
            file_sink,
        };

        const std::shared_ptr<spdlog::logger> internal_logger =
            std::make_shared<spdlog::logger>("HyperEngine", log_sinks.begin(), log_sinks.end());
        internal_logger->set_level(spdlog::level::trace);
        internal_logger->flush_on(spdlog::level::trace);

        spdlog::set_default_logger(internal_logger);
        spdlog::set_level(spdlog::level::info);
    }

    void set_level(const spdlog::level::level_enum level)
    {
        spdlog::set_level(level);
    }
} // namespace hyper_core::logger
