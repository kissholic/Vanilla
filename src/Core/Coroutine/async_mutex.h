/*
 * File: async_mutex.hpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#pragma once

#include <atomic>
#include <cassert>
#include <coroutine>
#include <mutex>

class async_mutex;
class async_mutex_lock;
class async_mutex_lock_operation;
class async_mutex_scoped_lock_operation;

class async_mutex {
  friend async_mutex_lock_operation;

public:
  async_mutex() noexcept : m_state{UNLOCKED}, m_waiters{nullptr} {}

  ~async_mutex() {
    std::uintptr_t state = m_state.load(std::memory_order_relaxed);
    assert(m_waiters == nullptr);
  }

  async_mutex(async_mutex const &) = delete;
  async_mutex &operator=(async_mutex const &) = delete;

  bool try_lock() noexcept;

  async_mutex_lock_operation lock_async() noexcept;

  async_mutex_scoped_lock_operation scoped_lock_async() noexcept;

  void unlock() noexcept;

private:
  static constexpr std::uintptr_t UNLOCKED = -1;
  static constexpr std::uintptr_t NOWAITER = 0;

  std::atomic<std::uintptr_t> m_state;
  async_mutex_lock_operation *m_waiters;
};

class async_mutex_lock_operation {
public:
  async_mutex_lock_operation(async_mutex &mtx) noexcept
      : m_mutex{mtx}, m_next{nullptr} {}

  bool await_ready() const noexcept { return false; }

  bool await_suspend(std::coroutine_handle<> awaiter) noexcept {
    m_coroutine = awaiter;

    while (true) {
      std::uintptr_t old_state =
          m_mutex.m_state.load(std::memory_order_acquire);
      if (old_state == async_mutex::UNLOCKED) {
        if (m_mutex.m_state.compare_exchange_weak(
                old_state, async_mutex::NOWAITER, std::memory_order_release,
                std::memory_order_relaxed)) {
          return false;
        }
      } else {
        m_next = reinterpret_cast<async_mutex_lock_operation *>(this);
        if (m_mutex.m_state.compare_exchange_weak(
                old_state, reinterpret_cast<std::uintptr_t>(this),
                std::memory_order_release, std::memory_order_relaxed)) {
          return true;
        }
      }
    }
  }

  void await_resume() const noexcept {}

protected:
  friend async_mutex;

  async_mutex &m_mutex;

  async_mutex_lock_operation *m_next;

  std::coroutine_handle<> m_coroutine;
};

class async_mutex_scoped_lock_operation : public async_mutex_lock_operation {
public:
  [[nodiscard]] async_mutex_lock await_resume() const noexcept;
};

class async_mutex_lock {
public:
  async_mutex_lock(async_mutex &mtx) noexcept : m_mutex{std::addressof(mtx)} {}

  async_mutex_lock(async_mutex_lock &&other) noexcept : m_mutex{other.m_mutex} {
    other.m_mutex = nullptr;
  }

  async_mutex_lock(async_mutex_lock const &) = delete;
  async_mutex_lock &operator=(async_mutex_lock const &) = delete;

  ~async_mutex_lock() {
    if (m_mutex) {
      m_mutex->unlock();
    }
  }

private:
  async_mutex *m_mutex;
};