#-------------------------------------------------------------------------------------------
# Copyright (c) 2024-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_event/hyper_event.cpp)

set(HEADERS
        include/hyper_event/event_bus.hpp
        include/hyper_event/event_handler.hpp
        include/hyper_event/event_id_generator.hpp
        include/hyper_event/hyper_event.hpp)

hyperengine_define_library(hyper_event)
target_link_libraries(
        hyper_event
        PUBLIC
        hyper_core)
