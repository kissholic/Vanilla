/*
 * File: Task.mpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#pragma once

#include <coroutine>
#include <exception>


class co_promise_base
{
    friend struct final_awaitable;

    struct final_awaitable
    {
        bool await_ready() const noexcept { return false; }

        template<typename T>
        std::coroutine_handle<> await_suspend(std::coroutine_handle<T> coro) noexcept
        {
            return coro.promise().m_continuation;
        }

        void await_resume() noexcept {}
    };

public:
    co_promise_base() noexcept { }

    auto inital_suspend() noexcept { return std::suspend_never{}; }

    auto final_suspend() noexcept { return final_awaitable{}; }

    void setj_cotinuation(std::coroutine_handle<> continuation)
    {
        m_continuation = continuation;
    }

private:
    std::coroutine_handle<> m_continuation;
};


template<typename T>
class co_task;

template<typename T>
class co_promise final : public co_promise_base
{
public:
    co_promise() noexcept {}

    ~co_promise()
    {
        switch (m_type)
        {
        case result_type::VALUE:
            m_value.~T();
            break;
        case result_type::EXCEPTION:
            break;
        default:
            break;
        }
    }

    co_task<T> get_return_object() noexcept;

    void unhandled_exception() noexcept
    {
        std::terminate();
    }

    template<std::convertible_to<T> ValueType>
    void return_value(ValueType&& value) noexcept
    {
        ::new (static_cast<void*>(std::addressof(m_value))) T(std::forward(value));
        m_type = result_type::VALUE;
    }

    T& result() & noexcept
    {
        if (m_type == result_type::EXCEPTION)
        {
            std::terminate();
        }

        assert(m_type == result_type::VALUE);

        return m_value;
    }

    T&& result() && noexcept
    {
        if (m_type == result_type::EXCEPTION)
        {
            std::terminate();
        }

        assert(m_type == result_type::VALUE);

        return std::move(m_value);
    }

private:
    enum class result_type { EMPTY, VALUE, EXCEPTION };

    result_type m_type = result_type::EMPTY;

    T m_value;
};


template<>
class co_promise<void> : public co_promise_base
{
public:
    co_promise() noexcept = default;

    co_task<void> get_return_object() noexcept;

    void return_void() noexcept {}

    void unhandled_exception() noexcept
    {
        std::terminate();
    }
};


template<typename T>
class co_promise<T&> : co_promise_base
{
public:
    co_promise() noexcept = default;

    co_task<T&> get_return_object() noexcept;

    void unhandled_exception() noexcept
    {
        std::terminate();
    }

    void return_value(T& value) noexcept
    {
        m_value = std::addressof(value);
    }

    T& result() noexcept
    {
        return *m_value;
    }

private:
    T* m_value = nullptr;
};


template<typename T = void>
class [[nodiscard]] co_task
{
public:
    using promise_type = co_promise<T>;
    using value_type = T;

private:
    struct awaitable_base
    {
        std::coroutine_handle<promise_type> m_coroutine;

        awaitable_base(std::coroutine_handle<promise_type> handle) noexcept
            : m_coroutine{handle}
        {

        }

        bool await_ready() const noexcept
        {
            return !m_coroutine || m_coroutine.done();
        }

        std::coroutine_handle<> await_suspend(std::coroutine_handle<> handle) noexcept
        {
            m_coroutine.promise().setj_cotinuation(handle);
            return m_coroutine;
        }
    };

public:

    co_task() noexcept
        : m_coroutine{nullptr}
    {

    }

    co_task(co_task&& task) noexcept
        : m_coroutine{task.m_coroutine}
    {
        task.m_coroutine = nullptr;
    }

    co_task(std::coroutine_handle<promise_type> handle) noexcept
        : m_coroutine{handle}
    {

    }

    ~co_task() noexcept
    {
        if (m_coroutine)
        {
            m_coroutine.destroy();
        }
    }

    auto& operator=(co_task&& task) noexcept
    {
        if (std::addressof(task) == this)
        {
            if (m_coroutine)
            {
                m_coroutine.destroy();
            }

            m_coroutine = task.m_coroutine;
            task.m_coroutine = nullptr;
        }

        return *this;
    }

    auto operator co_await() const & noexcept
    {
        struct awaitable : awaitable_base
        {
            decltype(auto) await_resume() noexcept
            {
                if (!this->m_coroutine)
                {
                    std::terminate();
                }

                return this->m_coroutine.promise().result();
            }
        };

        return awaitable{m_coroutine};
    }

    auto operator co_await() const && noexcept
    {
        struct awaitable : awaitable_base
        {
            decltype(auto) await_resume()
            {
                if (!this->m_coroutine)
                {
                    std::terminate();
                }

                return std::move(this->m_coroutine.promise()).result();
            }
        };

        return awaitable{m_coroutine};
    }

    bool ready() const noexcept
    {
        return !m_coroutine || m_coroutine.done();
    }

    auto when_ready() const noexcept
    {
        struct awaitable : awaitable_base
        {
            using awaitable_base::awaitable_base;

            void await_resume() const noexcept {}
        };

        return awaitable{m_coroutine};
    }

private:
    std::coroutine_handle<promise_type> m_coroutine;
};


template<typename T>
co_task<T> co_promise<T>::get_return_object() noexcept
{
    return co_task<T>{std::coroutine_handle<co_promise>::from_promise(*this)};
}

inline co_task<void> co_promise<void>::get_return_object() noexcept
{
    return co_task<void>{std::coroutine_handle<co_promise>::from_promise(*this)};
}

template<typename T>
co_task<T&> co_promise<T&>::get_return_object() noexcept
{
    return co_task<T&>{std::coroutine_handle<co_promise>::from_promise(*this)};
}

template<typename AWAITABLE>
auto make_co_task(AWAITABLE&& awaitable)
{
    co_return co_await static_cast<AWAITABLE&&>(awaitable);
}