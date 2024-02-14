/*
 * File: Vanilla.cpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#include "core/coroutine/generator.h"

// #include "imgui.h"
// #include "imgui/backends/imgui_impl_glfw.h"
// #include "imgui/backends/imgui_impl_metal.h"

// // #define GLFW_INCLUDE_NONE
// #include "GLFW/glfw3.h"
// #include "GLFW/glfw3native.h"

#include "mujoco/mujoco.h"
#include "spdlog/spdlog.h"
// #include "window/graphic_lib.h"
// #include "window/van_window.h"

char err[1000];
mjModel *m;
mjData *d;

int main(int argc, char *argv[]) {
  m = mj_loadXML("hello.xml", nullptr, err, 1000);
  if (!m) {
    spdlog::info("Failed to load xml %s", "hello.xml");
    return 1;
  }

  d = mj_makeData(m);

  spdlog::info("start");
  while (d->time < 1000) {
    mj_step(m, d);
  }

  spdlog::info("finish");

  mj_deleteData(d);
  mj_deleteModel(m);

  return 0;
}