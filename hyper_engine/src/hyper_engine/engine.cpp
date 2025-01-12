/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_engine/engine.hpp"

namespace hyper_engine
{
    const Scene &Engine::scene() const
    {
        return m_scene;
    }
} // namespace hyper_engine