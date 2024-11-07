#-------------------------------------------------------------------------------------------
# Copyright (c) 2024, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

add_library(
        stb
        src/stb_image.cpp
        ${CMAKE_BINARY_DIR}/_deps/stb-src/stb_image.h)
target_include_directories(
        stb
        SYSTEM
        PUBLIC
        ${CMAKE_BINARY_DIR}/_deps/stb-src/)

set_target_properties(
        stb
        PROPERTIES
        FOLDER "third_party")