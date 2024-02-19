/*
 * File: graphic_lib.cpp
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#include "graphic_lib.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

static void error_callback(int error, char const *msg) {
  spdlog::error("GLFW error code: {}, message: {}", error, msg);
}

bool graphic_lib::init_glfw() {
  if (!glfwInit()) {
    spdlog::error("GLFW init failed.");
    return false;
  }

  glfwSetErrorCallback(error_callback);

  return true;
}

void graphic_lib::fin_glfw() { glfwTerminate(); }