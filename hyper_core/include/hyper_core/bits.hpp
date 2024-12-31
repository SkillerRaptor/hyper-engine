/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <type_traits>

#define HE_BIT_8(x) (static_cast<uint8_t>(1) << (x))
#define HE_BIT_16(x) (static_cast<uint16_t>(1) << (x))
#define HE_BIT_32(x) (static_cast<uint32_t>(1) << (x))
#define HE_BIT_64(x) (static_cast<uint64_t>(1) << (x))

#define HE_MASK_8(x) (BIT8(x) - static_cast<uint8_t>(1))
#define HE_MASK_16(x) (BIT16(x) - static_cast<uint16_t>(1))
#define HE_MASK_32(x) (BIT32(x) - static_cast<uint32_t>(1))
#define HE_MASK_64(x) (BIT64(x) - static_cast<uint64_t>(1))

#define HE_ENABLE_BITMASK_OPERATORS(T)                                                                                           \
    inline constexpr T operator|(T left, T right)                                                                                \
    {                                                                                                                            \
        return static_cast<T>(static_cast<::std::underlying_type_t<T>>(left) | static_cast<::std::underlying_type_t<T>>(right)); \
    }                                                                                                                            \
                                                                                                                                 \
    inline constexpr T operator&(T left, T right)                                                                                \
    {                                                                                                                            \
        return static_cast<T>(static_cast<::std::underlying_type_t<T>>(left) & static_cast<::std::underlying_type_t<T>>(right)); \
    }                                                                                                                            \
                                                                                                                                 \
    inline constexpr T operator^(T left, T right)                                                                                \
    {                                                                                                                            \
        return static_cast<T>(static_cast<::std::underlying_type_t<T>>(left) ^ static_cast<::std::underlying_type_t<T>>(right)); \
    }                                                                                                                            \
                                                                                                                                 \
    inline constexpr T operator~(T value)                                                                                        \
    {                                                                                                                            \
        return static_cast<T>(~static_cast<::std::underlying_type_t<T>>(value));                                                 \
    }                                                                                                                            \
                                                                                                                                 \
    inline T &operator|=(T &left, T right)                                                                                       \
    {                                                                                                                            \
        left = static_cast<T>(static_cast<::std::underlying_type_t<T>>(left) | static_cast<::std::underlying_type_t<T>>(right)); \
        return left;                                                                                                             \
    }                                                                                                                            \
                                                                                                                                 \
    inline T &operator&=(T &left, T right)                                                                                       \
    {                                                                                                                            \
        left = static_cast<T>(static_cast<::std::underlying_type_t<T>>(left) & static_cast<::std::underlying_type_t<T>>(right)); \
        return left;                                                                                                             \
    }                                                                                                                            \
                                                                                                                                 \
    inline T &operator^=(T &left, T right)                                                                                       \
    {                                                                                                                            \
        left = static_cast<T>(static_cast<::std::underlying_type_t<T>>(left) ^ static_cast<::std::underlying_type_t<T>>(right)); \
        return left;                                                                                                             \
    }