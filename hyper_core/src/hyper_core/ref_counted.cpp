/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_core/ref_counted.hpp"

#include "hyper_core/assertion.hpp"

namespace hyper_engine
{
    RefCountedBase::~RefCountedBase()
    {
        HE_ASSERT(!m_ref_count);
    }

    void RefCountedBase::ref() const
    {
        HE_ASSERT(m_ref_count > 0);
        m_ref_count += 1;
    }

    bool RefCountedBase::deref_base() const
    {
        HE_ASSERT(m_ref_count);
        m_ref_count -= 1;
        return m_ref_count == 0;
    }
} // namespace hyper_engine