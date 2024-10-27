/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <spdlog/common.h>
#include <spdlog/logger.h>

namespace hyper_core
{
    class Logger
    {
    public:
        static void initialize();

        static void set_level(spdlog::level::level_enum level);

        static std::shared_ptr<spdlog::logger> &internal_logger();

    private:
        static std::shared_ptr<spdlog::logger> s_internal_logger;
    };
} // namespace hyper_core

#define HE_INFO(...) ::hyper_core::Logger::internal_logger()->info(__VA_ARGS__)
#define HE_WARN(...) ::hyper_core::Logger::internal_logger()->warn(__VA_ARGS__)
#define HE_ERROR(...) ::hyper_core::Logger::internal_logger()->error(__VA_ARGS__)
#define HE_CRITICAL(...) ::hyper_core::Logger::internal_logger()->critical(__VA_ARGS__)
#define HE_DEBUG(...) ::hyper_core::Logger::internal_logger()->debug(__VA_ARGS__)
#define HE_TRACE(...) ::hyper_core::Logger::internal_logger()->trace(__VA_ARGS__)
