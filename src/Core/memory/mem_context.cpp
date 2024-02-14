/*
 * File: mem_context.cpp
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#include "mem_context.h"

mem_context *mem_context::root_context = nullptr;

bin_mem_context::bin_mem_context(mem_context *parent) : mem_context{parent} {}

bin_mem_context::~bin_mem_context() {}

void *bin_mem_context::malloc(::std::size_t size) noexcept { return nullptr; }

void bin_mem_context::free(void *ptr) noexcept {}

void *bin_mem_context::realloc(void *ptr, ::std::size_t size) noexcept {
  return nullptr;
}

void *bin_mem_context::calloc(::std::size_t nmemb,
                              ::std::size_t size) noexcept {
  return nullptr;
}