/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include "hyper_core/global_environment.hpp"

namespace hyper_engine
{
    class Logger
    {
    public:
        Logger();

        void set_level(spdlog::level::level_enum level) const;

        const std::unique_ptr<spdlog::logger> &internal_logger() const;

    private:
        std::unique_ptr<spdlog::logger> m_internal_logger;
    };
} // namespace hyper_engine

#define HE_INFO(...) SPDLOG_LOGGER_CALL(g_env.logger->internal_logger(), spdlog::level::info, __VA_ARGS__)
#define HE_WARN(...) SPDLOG_LOGGER_CALL(g_env.logger->internal_logger(), spdlog::level::warn, __VA_ARGS__)
#define HE_ERROR(...) SPDLOG_LOGGER_CALL(g_env.logger->internal_logger(), spdlog::level::err, __VA_ARGS__)
#define HE_CRITICAL(...) SPDLOG_LOGGER_CALL(g_env.logger->internal_logger(), spdlog::level::critical, __VA_ARGS__)
#define HE_DEBUG(...) SPDLOG_LOGGER_CALL(g_env.logger->internal_logger(), spdlog::level::debug, __VA_ARGS__)
#define HE_TRACE(...) SPDLOG_LOGGER_CALL(g_env.logger->internal_logger(), spdlog::level::trace, __VA_ARGS__)
