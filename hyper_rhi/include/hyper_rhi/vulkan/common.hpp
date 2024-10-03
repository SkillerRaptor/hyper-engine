/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <volk.h>
#include <vulkan/vk_enum_string_helper.h>

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#define HE_VK_CHECK(x)                                                        \
    do                                                                        \
    {                                                                         \
        const VkResult result = x;                                            \
        if (result != VK_SUCCESS)                                             \
        {                                                                     \
            HE_FATAL("Vulkan operation failed: {}", string_VkResult(result)); \
            HE_UNREACHABLE();                                                 \
        }                                                                     \
    } while (0)
