/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <type_traits>

#define HE_ENABLE_BITMASK_OPERATORS(T)                                                                                       \
    inline constexpr T operator|(T left, T right)                                                                            \
    {                                                                                                                        \
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(left) | static_cast<std::underlying_type_t<T>>(right)); \
    }                                                                                                                        \
                                                                                                                             \
    inline constexpr T operator&(T left, T right)                                                                            \
    {                                                                                                                        \
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(left) & static_cast<std::underlying_type_t<T>>(right)); \
    }                                                                                                                        \
                                                                                                                             \
    inline constexpr T operator^(T left, T right)                                                                            \
    {                                                                                                                        \
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(left) ^ static_cast<std::underlying_type_t<T>>(right)); \
    }                                                                                                                        \
                                                                                                                             \
    inline constexpr T operator~(T value)                                                                                    \
    {                                                                                                                        \
        return static_cast<T>(~static_cast<std::underlying_type_t<T>>(value));                                               \
    }                                                                                                                        \
                                                                                                                             \
    inline T &operator|=(T &left, T right)                                                                                   \
    {                                                                                                                        \
        left = static_cast<T>(static_cast<std::underlying_type_t<T>>(left) | static_cast<std::underlying_type_t<T>>(right)); \
        return left;                                                                                                         \
    }                                                                                                                        \
                                                                                                                             \
    inline T &operator&=(T &left, T right)                                                                                   \
    {                                                                                                                        \
        left = static_cast<T>(static_cast<std::underlying_type_t<T>>(left) & static_cast<std::underlying_type_t<T>>(right)); \
        return left;                                                                                                         \
    }                                                                                                                        \
                                                                                                                             \
    inline T &operator^=(T &left, T right)                                                                                   \
    {                                                                                                                        \
        left = static_cast<T>(static_cast<std::underlying_type_t<T>>(left) ^ static_cast<std::underlying_type_t<T>>(right)); \
        return left;                                                                                                         \
    }