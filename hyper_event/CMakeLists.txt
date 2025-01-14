#-------------------------------------------------------------------------------------------
# Copyright (c) 2024-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES)

set(HEADERS
        include/hyper_event/event_bus.hpp
        include/hyper_event/event_handler.hpp
        include/hyper_event/event_id_generator.hpp)

hyperengine_define_library(hyper_event)
target_link_libraries(
        hyper_event
        INTERFACE
        hyper_core)
