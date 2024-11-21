/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <spdlog/spdlog.h>

namespace hyper_engine::logger
{
    void initialize();
    void shutdown();

    void set_level(spdlog::level::level_enum level);

    const std::unique_ptr<spdlog::logger> &internal_logger();
} // namespace hyper_engine::logger

#define HE_INFO(...) SPDLOG_LOGGER_CALL(::hyper_engine::logger::internal_logger(), spdlog::level::info, __VA_ARGS__)
#define HE_WARN(...) SPDLOG_LOGGER_CALL(::hyper_engine::logger::internal_logger(), spdlog::level::warn, __VA_ARGS__)
#define HE_ERROR(...) SPDLOG_LOGGER_CALL(::hyper_engine::logger::internal_logger(), spdlog::level::err, __VA_ARGS__)
#define HE_CRITICAL(...) SPDLOG_LOGGER_CALL(::hyper_engine::logger::internal_logger(), spdlog::level::critical, __VA_ARGS__)
#define HE_DEBUG(...) SPDLOG_LOGGER_CALL(::hyper_engine::logger::internal_logger(), spdlog::level::debug, __VA_ARGS__)
#define HE_TRACE(...) SPDLOG_LOGGER_CALL(::hyper_engine::logger::internal_logger(), spdlog::level::trace, __VA_ARGS__)
