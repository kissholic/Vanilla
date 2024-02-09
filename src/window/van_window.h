/*
 * File: van_window.h
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#pragma once

#include "GLFW/glfw3.h"
#include <string>

class van_window {
public:
  van_window(::std::string const &title, unsigned int width,
             unsigned int height);

  virtual ~van_window();

  void set_current() noexcept;

  bool should_close() noexcept;

  operator bool() noexcept;

  void tick() noexcept;

  GLFWwindow *get_native() noexcept { return m_window; }

private:
  std::string m_title;
  GLFWwindow *m_window;
};