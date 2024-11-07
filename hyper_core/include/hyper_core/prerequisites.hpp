/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#define HE_UNUSED(x) ((void) (x))

#define HE_STRINGIFY_HELPER(x) #x
#define HE_STRINGIFY(x) HE_STRINGIFY_HELPER(x)
#define HE_EXPAND_MACRO(x) x

#define HE_BIND_FUNCTION(function)                                    \
    [this](auto &&...args) -> decltype(auto)                          \
    {                                                                 \
        return this->function(std::forward<decltype(args)>(args)...); \
    }

#ifndef HE_FORCE_INLINE
#    if defined(__GNUC__)
#        define HE_FORCE_INLINE __attribute__((always_inline)) inline
#    elif defined(_MSC_VER)
#        define HE_FORCE_INLINE __forceinline
#    else
#        define HE_FORCE_INLINE inline
#    endif
#endif

#ifndef NDEBUG
#    define HE_DEBUG_BUILD 1
#else
#    define HE_RELEASE_BUILD 1
#endif
