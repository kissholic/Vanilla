/*
 * File: Vanilla.cpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#include "core/coroutine/generator.h"

// #include "imgui.h"
// #include "imgui/backends/imgui_impl_glfw.h"
// #include "imgui/backends/imgui_impl_metal.h"

// #include "GLFW/glfw3native.h"

#include "spdlog/spdlog.h"
#include "window/graphic_lib.h"
#include "window/van_window.h"

int main(int argc, char *argv[]) {
  graphic_lib grah_lib{};

  if (!glfwVulkanSupported()) {
    spdlog::info("vulkan not supported.");
  }
  return 0;
}
