/*
 * File: async_mutex.cpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#include "async_mutex.h"



async_mutex_lock_operation async_mutex::lock_async() noexcept
{
    return async_mutex_lock_operation{*this};
}


async_mutex_scoped_lock_operation async_mutex::scoped_lock_async() noexcept
{
    return async_mutex_scoped_lock_operation{*this};
}


void async_mutex::unlock() noexcept
{
    assert(m_state.load(std::memory_order_acquire) != UNLOCKED);

    auto* waiter = m_waiters;
    if (waiter == nullptr)
    {
        auto old_state = NOWAITER;
        if (m_state.compare_exchange_strong(
            old_state,
            UNLOCKED,
            std::memory_order_release,
            std::memory_order_relaxed
        ))
        {
            return;
        }
        
        old_state = m_state.exchange(NOWAITER, std::memory_order_acquire);
        assert(old_state != UNLOCKED && old_state != NOWAITER);

        auto* next = reinterpret_cast<async_mutex_lock_operation*>(old_state);
        do {
            auto* temp = next->m_next;
            next->m_next = waiter;
            waiter = next;
            next = temp;
        } while (next);
    }

    assert(waiter != nullptr);

    m_waiters = waiter->m_next;
    waiter->m_coroutine.resume();
}


[[nodiscard]] async_mutex_lock async_mutex_scoped_lock_operation::await_resume() const noexcept
{
    return async_mutex_lock{m_mutex};
}
