/*
 * File: mem_context.h
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

class mem_context {
private:
  // TODO: Refactor for multi-threads.
  static mem_context *root_context = nullptr;

public:
  mem_context(mem_context *parent) : m_parent{parent} {
    if (m_parent)
      m_parent->add_subcontext(this);
  }

  virtual ~mem_context() {
    // exit current context
    root_context = m_parent;
  }

  mem_context(mem_context const &) = delete;
  mem_context(mem_context &&) = delete;

  mem_context &operator=(mem_context const &) = delete;
  mem_context &operator=(mem_context &&) = delete;

  virtual void *malloc(::std::size_t size) noexcept = 0;
  virtual void free(void *ptr) noexcept = 0;
  virtual void *realloc(void *ptr, ::std::size_t size) noexcept = 0;
  virtual void *calloc(::std::size_t nmemb, ::std::size_t size) noexcept = 0;
  virtual void *align_alloc(::std::size_t alignment,
                            ::std::size_t size) noexcept {
    return nullptr;
  }

protected:
  void add_subcontext(mem_context *context) noexcept {
    m_subcontexts.push_back(context);
  }

private:
  mem_context *m_parent;
  ::std::vector<::std::unique_ptr<mem_context>> m_subcontexts;
};

// trivial_mem_context won't recycle allocated memory when deallocating!
// This should be the class of root context.
class trivial_mem_context : public mem_context {
public:
  virtual trivial_mem_context() : mem_context(nullptr) {}

  virtual void *malloc(::std::size_t size) noexcept override {
    return ::std::malloc(size);
  }

  virtual void free(void *ptr) { return ::std::free(ptr); }

  virtual void *realloc(void *ptr, ::std::size_t size) noexcept override {
    return ::std::realloc(ptr, size);
  }

  virtual void *calloc(::std::size_t nmemb,
                       ::std::size_t size) noexcept override {
    return ::std::calloc(nmemb, size);
  }

  virtual void *align_alloc(::std::size_t alignment,
                            ::std::size_t size) noexcept {
    return ::std::aligned_alloc(alignment, size);
  }

private:
};

// mono_mem_context allocates N bytes memory block, and holds it until
// destructor
class mono_mem_context : public mem_context {
public:
  mono_mem_context(mem_context *parent, unsigned int alignment,
                   unsigned int capacity)
      : m_parent{parent}, m_storage{::std::make_unique<char[]>(
                              m_parent->aligned_alloc(alignment, capacity))},
        m_capacity{capacity}, m_occupied{0} {}

  virtual void *malloc(::std::size_t size) noexcept {
    unsigned int old_mark = m_mark;
    m_mark += size;
    assert(m_mark <= m_capacity);
    return &m_storage[old_mark];
  }

  virtual void free(void *ptr) noexcept {}

  virtual void *realloc(void *ptr, ::std::size_t size) noexcept {
    this->malloc(size);
  }

  virtual void *calloc(::std::size_t nmemb, ::std::size_t size) noexcept {
    unsigned int total = nmemb * size;
    auto *ptr = this->malloc(total);
    ::std::memset(ptr, 0, total);
    return ptr;
  }

private:
  ::std::unique_ptr<char[]> m_storage;
  unsigned int m_capacity;
  unsigned int m_mark;
};
