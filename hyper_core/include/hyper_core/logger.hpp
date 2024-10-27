/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace hyper_core::logger
{
    void initialize();

    void set_level(spdlog::level::level_enum level);
} // namespace hyper_core::logger

#define HE_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define HE_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define HE_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define HE_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define HE_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define HE_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
