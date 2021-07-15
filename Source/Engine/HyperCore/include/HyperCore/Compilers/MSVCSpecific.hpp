/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#if !defined(_MSC_VER)
#	error This file should only be included on the vc compilers
#endif

#define HYPERENGINE_COMPILER_VC 1

#define HYPERENGINE_COMPILER_PUSH_WARNING _Pragma(warning(push))
#define HYPERENGINE_COMPILER_POP_WARNING _Pragma(warning(pop))
#define HYPERENGINE_COMPILER_PUSH_ANONYMOUS_STRUCT _Pragma(warning(disable : 4201))

#define HYPERENGINE_ALIGN(bytes) __declspec(align(bytes))
