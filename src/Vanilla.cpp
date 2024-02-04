/*
 * File: Vanilla.cpp
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#include "core/coroutine/generator.h"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "window/graphic_lib.h"
#include "window/van_window.h"

co_generator<int> test() {
  int i = 0;
  for (int i = 0; i < 10; i++)
    co_yield i;
}

int main(int argc, char *argv[]) {
  graphic_lib gl;
  van_window window{"Vanila", 640, 480};

  while (!window) {
    window.tick();
  }

  return 0;
}