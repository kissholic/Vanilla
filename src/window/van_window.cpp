/*
 * File: van_window.cpp
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#include "van_window.h"
#include "spdlog/spdlog.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

van_window::van_window(std::string const &title, unsigned int width,
                       unsigned int height)
    : m_window(nullptr) {
  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  if (!m_window) {
    glfwTerminate();
    spdlog::error("Creating window failed.");
    return;
  }

  glfwSetKeyCallback(m_window, key_callback);

  set_current();

  spdlog::info("Window {} created.", title.c_str());
}

van_window::~van_window() {
  glfwDestroyWindow(m_window);
  spdlog::info("Window {} destroyed", m_title.c_str());
}

void van_window::set_current() noexcept { glfwMakeContextCurrent(m_window); }

bool van_window::should_close() noexcept {
  return glfwWindowShouldClose(m_window);
}

van_window::operator bool() noexcept { return should_close(); }

void van_window::tick() noexcept {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}