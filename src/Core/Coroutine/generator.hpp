/*
 * File: generator.mpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#pragma once

#include <coroutine>
#include <exception>

template<typename T>
class co_generator;


namespace detail{


template<typename T>
class generator_promise
{
public:
    generator_promise() noexcept {}

    auto initial_suspend() noexcept { return std::suspend_always{}; }

    auto final_suspend() noexcept { return std::suspend_always{}; }

    void unhandled_exception() noexcept
    {
        std::terminate();
    }

    auto get_return_object() noexcept
    {
        return co_generator<T>{std::coroutine_handle<generator_promise>::from_promise(*this)};
    }

    template<std::convertible_to<T> From>
    std::suspend_always yield_value(From&& value) noexcept
    {
        m_value = std::forward<From>(value);
        return {};
    }

    void return_void() noexcept {}

    T& value() noexcept
    {
        return m_value;
    }

private:
    T m_value;
};

}


template<typename T>
class [[nodiscard]] co_generator
{
public:
    using promise_type = detail::generator_promise<T>;
    using value_type = T;

    class sentinel{};

    class iterator
    {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::remove_reference_t<T>;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = std::size_t;

        iterator() noexcept
            : m_coroutine{nullptr}
        {}

        iterator(std::coroutine_handle<promise_type> other) noexcept
            : m_coroutine{other}
        {
        }
        
        iterator& operator++() noexcept
        {
            m_coroutine.resume();
            return *this;
        }

        void operator++(int) noexcept
        {
            (void) operator++();
        }

        reference operator*() const noexcept
        {
            return m_coroutine.promise().value();
        }

        pointer operator->() const noexcept
        {
            return std::addressof(operator*());
        }

        friend bool operator==(iterator const& it, sentinel) noexcept
        {
            return !it.m_coroutine || it.m_coroutine.done();
        }

        friend bool operator!=(iterator const& it, sentinel s) noexcept
        {
            return !(it == s);
        }

    private:
        std::coroutine_handle<promise_type> m_coroutine;
    };

    co_generator() noexcept
        : m_coroutine{nullptr}
    {}

    co_generator(co_generator&& other) noexcept
        : m_coroutine{other.m_coroutine}
    {
        other.m_coroutine = nullptr;
    }

    co_generator(std::coroutine_handle<promise_type> other) noexcept
        : m_coroutine{other}
    {

    }

    co_generator(co_generator const&) = delete;

    co_generator& operator=(co_generator&& other) noexcept
    {
        m_coroutine = other.m_coroutine;
        other.m_coroutine = nullptr;
    }

    co_generator& operator=(co_generator const&) = delete;

    ~co_generator()
    {
        if (m_coroutine)
        {
            m_coroutine.destroy();
        }
    }

    iterator begin() noexcept
    {
        if (m_coroutine)
        {
            m_coroutine.resume();
        }
        return iterator{m_coroutine};
    }

    sentinel end() noexcept
    {
        return {};
    }


private:
    std::coroutine_handle<promise_type> m_coroutine;
};