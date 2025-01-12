#-------------------------------------------------------------------------------------------
# Copyright (c) 2024-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_ecs/hyper_ecs.cpp)

set(HEADERS
        include/hyper_ecs/hyper_ecs.hpp
        include/hyper_ecs/model_component.hpp
        include/hyper_ecs/transform_component.hpp)

hyperengine_define_library(hyper_ecs)
target_link_libraries(
        hyper_ecs
        PUBLIC
        hyper_core)

